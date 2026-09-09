#include "database_manager.h"

#include "base.h"
#include "mainmenu.h"
#include "settings.h"
#include "widget/w_database_creator.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>

Database_Manager::Database_Manager(QObject* parent)
  : QObject{parent}
{
}

bool Database_Manager::change_database(std::unique_ptr<Database> new_db)
{
  // if not the first database loaded
  if (!db) {
    QString msg = QObject::tr(
                      "Do you really want to load the backup file '%1' dated [%2]?\n"
                      "Located at : %3\n"
                      "This action will overwrite the current data!\n%4")
                      .arg(new_db->get_settings().get_name(),
                           new_db->get_settings().get_save_time().toString("dd/MM/yyyy HH:mm:ss"),
                           new_db->get_file_path(), TXT::WARNING_OPERATION);

    auto result = QMessageBox::warning(nullptr, "Load Confirmation", msg, QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) return false;
  }

  if (!new_db) return false;

  // save current db before unloading
  if (DB_MANAGER.get_db()) {
    DB_MANAGER.get_db()->save("auto unloading");
    // close and delete old db connexion
    if (QSqlDatabase::contains(DB_MANAGER.get_sql_db().databaseName())) {
      QSqlDatabase old_db = QSqlDatabase::database(DB_MANAGER.get_sql_db().databaseName());
      old_db.close();
      QSqlDatabase::removeDatabase(DB_MANAGER.get_sql_db().databaseName());
    }
  }

  if (!QFile::exists(new_db->get_file_path())) {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
                          QObject::tr("The folder database dosen't exist: %1").arg(new_db->get_file_path()));
    return false;
  }

  // if necessary
  QDir().mkpath(QFileInfo(DATABASE_PATH()).absolutePath());

  // create new db connexion to the loaded db
  new_db->get_sql_db().setDatabaseName(DATABASE_PATH());
  if (!new_db->get_sql_db().open()) {
    QMessageBox::critical(nullptr, QObject::tr("Error"),
                          QObject::tr("Impossible to open the database : %1").arg(DATABASE_PATH()));
    return false;
  }

  SETTINGS.set_last_db(new_db->get_file_name());
  SETTINGS.save();

  db = std::move(new_db);

  signal_db_changed();

  connect(db.get(), &Database::signal_db_updated, [this]() { signal_db_updated(); });

  return true;
}

QSqlDatabase Database_Manager::get_sql_db()
{
  if (db) return db->get_sql_db();
  return QSqlDatabase();
}

bool Database_Manager::is_valid(bool is_silent)
{
  if (!is_silent && !db.get()) {
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
      auto w_new = new W_Database_Creator(nullptr, "");
      w_new->setModal(true);
      w_new->exec();
    } else if (w.clickedButton() == load_db) {
      DB_MANAGER.get_mainmenu()->open_save_menu();
      w.close();
    } else if (w.clickedButton() == cancel) {
      w.close();
    }

    return false;
  }
  return db.get();
}
