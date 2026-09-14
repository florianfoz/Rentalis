#include "database/manager.h"

#include "base.h"
#include "database/database.h"
#include "database/manifest.h"
#include "mainmenu.h"
#include "rentalis_settings.h"
#include "w_database_creator.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>

Database_Manager::Database_Manager(QObject* parent)
  : QObject{parent}
{
}

bool Database_Manager::load_database(const QString& db_path)
{
  auto* new_db = Database::open_database(db_path);

  // if not the first database loaded
  if (!database && !WAIT_DATABASE_TO_START) {
    QString msg = QObject::tr(
                      "Do you really want to load the database '%1' dated [%2]?\n"
                      "Located at : %3\n"
                      "This action will overwrite the current data!\n%4")
                      .arg(new_db->manifest().name, new_db->manifest().last_save_time.toString("dd/MM/yyyy HH:mm:ss"),
                           new_db->file_path(), TXT::WARNING_OPERATION);

    auto result = QMessageBox::warning(nullptr, "Load Confirmation", msg, QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) goto cancel;
  }

  if (!new_db) return false;

  // save current db before unloading
  if (Database_Manager::current_database()) {
    Database_Manager::current_database()->save("auto unloading");
    // close and delete old db connexion
    if (QSqlDatabase::contains(Database_Manager::current_sql().databaseName())) {
      QSqlDatabase old_db = QSqlDatabase::database(Database_Manager::current_sql().databaseName());
      old_db.close();
      QSqlDatabase::removeDatabase(Database_Manager::current_sql().databaseName());
    }
  }

  if (!QFile::exists(new_db->file_path())) {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
                          QObject::tr("The folder database dosen't exist: %1").arg(new_db->file_path()));
    goto cancel;
  }

  // if necessary
  QDir().mkpath(QFileInfo(DATABASE_PATH()).absolutePath());

  // create new db connexion to the loaded db
  if (!new_db->sql().open()) {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
                          QObject::tr("Impossible to open the database : %1").arg(DATABASE_PATH()));
    return false;
  }

  RentalisSettings::last_database = new_db->file_name();
  RentalisSettings::save();

  delete database;
  database = new_db;

  instance().signal_db_changed();

  connect(database, &Database::signal_db_updated, []() { database->signal_db_updated(); });

  return true;

cancel:
  delete new_db;
  return false;
}

QSqlDatabase Database_Manager::current_sql()
{
  if (database) return database->sql();
  return {};
}

bool Database_Manager::is_valid(bool is_silent)
{
  if (!is_silent && !database) {
    QMessageBox w(nullptr);
    w.setIcon(QMessageBox::Warning);
    w.setWindowTitle(tr("Non-Existent Database"));
    w.setText(tr("No database loaded, please create a new database or load a new database from saves."));

    QPushButton* new_db  = w.addButton(tr("New database"), QMessageBox::ActionRole);
    QPushButton* load_db = w.addButton(tr("Load save"), QMessageBox::ActionRole);
    QPushButton* cancel  = w.addButton(tr("Cancel"), QMessageBox::RejectRole);

    w.setModal(true);
    w.exec();

    if (w.clickedButton() == new_db) {
      auto* w_new = new W_Database_Creator(nullptr, "");
      w_new->setModal(true);
      w_new->exec();
    } else if (w.clickedButton() == load_db) {
      Database_Manager::mainmenu()->open_database_menu();
      w.close();
    } else if (w.clickedButton() == cancel) {
      w.close();
    }

    return false;
  }
  return database;
}
