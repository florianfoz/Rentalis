#include "savemanager.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "preferences.h"

#include <QObject>
#include <QPushButton>


QTimer SaveManager::autosave_timer = QTimer();

void SaveManager::engage_autosave()
{
  autosave_timer.stop();

  if (!Preferences::autosave) return;

  const int interval = Preferences::autosave_frequency_minutes * 60 * 1000;

  if (interval <= 0) return;

  QObject::connect(&autosave_timer, &QTimer::timeout, [] {
    save_current(Preferences::autosave_override ? ESaveMode::override : ESaveMode::new_save, "autosave");
  });

  autosave_timer.start(interval);
}

bool SaveManager::save_current(ESaveMode mode, const QString& method)
{
  if (!Database_Manager::database) return false;

  if (mode == ESaveMode::NONE) {
    QMessageBox box;
    box.setIcon(QMessageBox::Question);
    box.setWindowTitle(QObject::tr("Save"));
    box.setText(QObject::tr("Do you want to save the changes you made to current database %1 last save ?\n"
                            "You changes will be lost if you don't save them.")
                    .arg(Database_Manager::current_database()->manifest.print_info()));

    auto* overrideButton = box.addButton(QObject::tr("Override"), QMessageBox::AcceptRole);
    auto* cancelButton   = box.addButton(QObject::tr("Cancel"), QMessageBox::RejectRole);
    auto* newSaveButton  = box.addButton(QObject::tr("New save"), QMessageBox::ActionRole);
    auto* dontSaveButton = box.addButton(QObject::tr("Don't save"), QMessageBox::ActionRole);

    box.exec();
    if (box.clickedButton() == overrideButton)
      mode = ESaveMode::override;
    else if (box.clickedButton() == cancelButton)
      return false;
    else if (box.clickedButton() == newSaveButton)
      mode = ESaveMode::new_save;
    else if (box.clickedButton() == dontSaveButton)
      mode = ESaveMode::NONE;
  }

  auto last_save_time = QDateTime::currentDateTime();


  QFile src(Database_Manager::current_database()->path);
  auto  dest = Database_Manager::current_database()->folder_path();


  switch (mode) {
  case ESaveMode::NONE:     break;
  case ESaveMode::new_save: {
    Database_Manager::current_database()->unload();

    if (!copy_dir(src.fileName(), dest)) {
      QMessageBox::critical(nullptr, QObject::tr("Save error"),
                            QObject::tr(R"(Cannot copy the save folder "%1" to "%2")").arg(src.fileName(), dest));
      return false;
    }

    break;
  }
  case ESaveMode::override: {
    Database_Manager::current_database()->unload();

    auto dest = Database_Manager::current_database()->folder_path();

    if (!copy_dir(src.fileName(), dest)) {
      QMessageBox::critical(nullptr, QObject::tr("Save error"),
                            QObject::tr(R"(Cannot copy the save folder "%1" to "%2")").arg(src.fileName(), dest));
      return false;
    }

    if (src.remove()) {
      QMessageBox::critical(nullptr, QObject::tr("Delete error"),
                            QObject::tr(R"(Cannot dete the save folder "%1")").arg(src.fileName()));
    }
    break;
  }
  case ESaveMode::no_save: return false;
  }


  delete Database_Manager::database;
  Database_Manager::database = nullptr;


  return load_save(dest);
}

bool SaveManager::export_save(const QString& src, const QString& dest)
{
  QFile   file(src);
  QString destination = dest + "/" + QString(file.filesystemFileName().stem().c_str());

  if (!copy_dir(src, destination)) {
    QMessageBox::critical(nullptr, QObject::tr("Save error"),
                          QObject::tr("Cannot copy the save folder \"%1\" to \"%2\"\nError: %3)")
                              .arg(src, destination, file.errorString()));
    return false;
  }

  if (auto manifest = Database_Manifest::load(destination + "/manifest.conf")) {
    manifest.last_save_time = QDateTime::currentDateTime();
    manifest.save("exported");
  }

  return true;
}

bool SaveManager::delete_save(const QString& path)
{
  auto result = QMessageBox::warning(
      nullptr, QObject::tr("Delete save"),
      QObject::tr("Do you want to delete the save located at \"%1\" ?\nYou can restore this save from the trash.")
          .arg(path),
      QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No);

  if (result != QMessageBox::Yes) return false;

  QFile src(path);
  if (!src.moveToTrash()) {
    QMessageBox::critical(nullptr, QObject::tr("Delete error"),
                          QObject::tr(R"(Cannot dete the save folder "%1")").arg(src.fileName()));
    return false;
  }

  Database_Manager::instance().signal_db_updated(ETable::NONE);

  return true;
}


bool SaveManager::load_save(const QString& path)
{
  (void)save_current(ESaveMode::NONE, "unload");


  auto* last = Database_Manager::database;

  Database_Manager::database = new Database(Database::load(path));

  if (Database_Manager::database->is_valid()) {
    if (last) last->unload();
    delete last;
    Database_Manager::instance().signal_db_changed();
    return true;
  }

  Database_Manager::database = last;
  return false;
}