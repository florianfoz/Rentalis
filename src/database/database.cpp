#include "database/database.h"

#include "base.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "database/recorder.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlIndex>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QUuid>


Database Database::create(const QString& _name, const QString& _description, const QString& _currency_iso,
                          const QLocale& _language)
{
  auto    dt            = QDateTime::currentDateTime();
  QString save_path     = SAVE_PATH() + "/" + _name + "/" + dt.toString("yyyy-MM-dd_HH-mm-ss");
  QString manifest_path = save_path + "/manifest.conf";
  QString db_path       = save_path + "/rentalis.sqlite";

  QDir().mkpath(save_path);

  QDir().mkpath(save_path + "/attachments/damages");
  QDir().mkpath(save_path + "/attachments/invoices");
  QDir().mkpath(save_path + "/attachments/landlords");
  QDir().mkpath(save_path + "/attachments/lease_agreements");
  QDir().mkpath(save_path + "/attachments/maintenances");
  QDir().mkpath(save_path + "/attachments/properties");
  QDir().mkpath(save_path + "/attachments/receipts");
  QDir().mkpath(save_path + "/attachments/tenants");


  {
    // template copy
    QFile ftemplate(":/database/database_template.sqlite");
    if (!ftemplate.open(QIODevice::ReadOnly)) {
      qCritical() << "Impossible to open the template";
      return {};
    }
    QFile out(db_path);
    if (!out.open(QIODevice::WriteOnly)) {
      qCritical() << "Impossible to create the file :" << db_path;
      return {};
    }
    out.write(ftemplate.readAll());
    out.close();
    ftemplate.close();
  }

  auto db = Database();

  // init manifest config default values
  db.manifest                = Database_Manifest::create(manifest_path, _name, _description);
  db.manifest.currency       = CurrencyInfo::from_iso(_currency_iso);
  db.manifest.language       = _language;
  db.manifest.last_save_time = dt; // avoid potential date time difference
  db.manifest.save("creation");    // save manifest

  // create SQLite connexion
  db.recorder = Database_Recorder::create(db_path, db.manifest.uuid);

  db.database_valid = true;

  db.path = save_path;

  emit Database_Manager::instance().signal_db_updated(ETable::NONE);
  return db;
}

Database Database::load(const QString& path)
{
  if (!QFile(path).exists()) return {};

  auto db = Database();
  db.path = path;

  db.manifest = Database_Manifest::load(path + "/manifest.conf");
  db.recorder = Database_Recorder::load(path + "/rentalis.sqlite", db.manifest.uuid);

  db.database_valid = db.manifest.is_valid();

  return db;
}

void Database::unload()
{
  manifest = Database_Manifest();
  recorder.unload();
  database_valid = false;
  path.clear();
}

QString Database::folder_path() const
{
  return SAVE_PATH() + "/" + folder_name();
}

QString Database::file_path() const
{
  return folder_path() + "/" + file_name();
}

QString Database::manifest_path() const
{
  return folder_path() + "/manifest.conf";
}

QString Database::folder_name() const
{
  return sanitize_fileName(manifest.name) + "/" + manifest.last_save_time.toString("yyyy-MM-dd_HH-mm-ss");
}

QString Database::file_name() const
{
  return "rentalis.sqlite";
}


bool Database::check_db_integrity() const
{
  const QString connName = manifest.uuid;

  return QSqlDatabase::contains(connName);
}
