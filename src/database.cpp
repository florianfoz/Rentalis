#include "database.h"

#include "base.h"
#include "db_settings.h"
#include "settings.h"

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

bool Database::query_check(QSqlQuery* query, const QString& tag)
{
  if (!query->exec()) {
    qDebug() << "Error on query exec: " << tag << ": " << query->lastError();
    return false;
  }
  return true;
}

Database::Database(QObject* parent)
  : QObject{parent}
{
}

Database::Database(const QString& _folder_name)
{
  open_database(_folder_name);
}

void Database::save(const QString& _save_method)
{
  if (!database_valid) {
    qWarning() << "Cannot save an invalid database.";
    return;
  }

  const QDateTime save_time = QDateTime::currentDateTime();

  settings.set_save_time(save_time);
  settings.save(_save_method);

  emit signal_db_updated();
}

bool Database::update_metadata(const QString& _save_method)
{
  if (!database_valid) {
    qWarning() << "Cannot update metadata of an invalid database.";
    return false;
  }

  const QString old_folder_path = get_database_path();

  if (!QDir(old_folder_path).exists()) {
    qCritical() << "Database folder does not exist:" << old_folder_path;
    return false;
  }

  const QString old_file_path = get_file_path();

  // Save current metadata first.
  settings.set_save_time(QDateTime::currentDateTime());
  settings.save(_save_method);

  const QString new_folder_path = get_database_path();
  const QString new_file_path   = get_file_path();

  // Nothing changed physically.
  if (old_folder_path == new_folder_path) {
    return true;
  }

  qDebug() << "Updating database metadata:";
  qDebug() << "Old path:" << old_folder_path;
  qDebug() << "New path:" << new_folder_path;

  /*
   * SQLite connection must be closed before moving the database,
   * especially when WAL mode is enabled.
   */
  const QString connection_name = settings.get_uuid();

  if (QSqlDatabase::contains(connection_name)) {
    QSqlDatabase db = QSqlDatabase::database(connection_name);

    if (db.isOpen()) {
      db.close();
    }

    // Remove the Qt connection only after all QSqlQuery/QSqlDatabase
    // objects referring to it have gone out of scope.
    QSqlDatabase::removeDatabase(connection_name);
  }

  // Rename the whole database directory.
  QDir save_dir(SAVE_PATH());

  if (!save_dir.rename(old_folder_path, new_folder_path)) {
    qCritical() << "Cannot rename database folder from" << old_folder_path << "to" << new_folder_path;

    // Try to reopen the original connection.
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connection_name);
    db.setDatabaseName(old_file_path);

    if (!db.open()) {
      database_valid = false;
      qCritical() << "Cannot reopen database after failed metadata update:" << db.lastError().text();
    }

    return false;
  }

  // Recreate the SQLite connection with the new file path.
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connection_name);
  db.setDatabaseName(new_file_path);

  if (!db.open()) {
    qCritical() << "Database folder was renamed, but SQLite cannot reopen:" << db.lastError().text();

    database_valid = false;
    return false;
  }

  // Restore SQLite configuration.
  QSqlQuery pragma(db);
  pragma.exec("PRAGMA journal_mode=WAL;");
  pragma.exec("PRAGMA busy_timeout=5000;");

  database_valid = true;

  emit signal_db_updated();

  return true;
}

void Database::remove()
{
  if (!database_valid) {
    qWarning() << "Cannot remove an invalid database.";
    return;
  }

  const QString database_path = get_database_path();

  if (!QDir(database_path).exists()) {
    qWarning() << "Database folder does not exist:" << database_path;
    database_valid = false;
    return;
  }

  const auto result = QMessageBox::warning(nullptr, QObject::tr("Database Deletion"),
                                           QObject::tr("Do you really want to permanently delete "
                                                       "the database?\n\n%1\n\n%2")
                                               .arg(database_path, TXT::WARNING_OPERATION),
                                           QMessageBox::Yes | QMessageBox::Cancel);

  if (result == QMessageBox::Cancel) {
    return;
  }

  const QString connection_name = settings.get_uuid();

  // Close SQLite connection before deleting its files.
  if (QSqlDatabase::contains(connection_name)) {
    QSqlDatabase db = QSqlDatabase::database(connection_name);

    if (db.isOpen()) {
      db.close();
    }

    QSqlDatabase::removeDatabase(connection_name);
  }

  // Remove the complete database directory, including attachments.
  QDir database_dir(database_path);

  if (!database_dir.removeRecursively()) {
    qCritical() << "Cannot delete database directory:" << database_path;

    QMessageBox::critical(nullptr, QObject::tr("Deletion Failed"),
                          QObject::tr("The database could not be completely deleted.\n%1").arg(database_path));

    return;
  }

  database_valid = false;

  emit signal_db_updated();
}

bool Database::open_database(const QString& _folder_name)
{
  if (_folder_name.isEmpty()) return false;

  QString db_path = SAVE_PATH() + "/" + _folder_name;

  // if no database existing : build a new database by coping the template
  if (!QFile::exists(db_path)) {
    QMessageBox::warning(nullptr, tr("Open Database Error"),
                         tr("Impossible to open the database at %1\nThe database dosen't exists.").arg(db_path));
    return false;
  }

  // test settings
  QString settings_file_path = db_path + "/config.ini";
  settings                   = DB_Settings(settings_file_path);
  if (!settings.is_valid()) {
    qCritical() << "Cannot opening the database settings file at " << settings_file_path << " in QSettings";
    return false;
  }
  QSqlDatabase::addDatabase("QSQLITE", settings.get_uuid());

  database_valid = true;

  return true;
}

bool Database::new_database(const QString& _name, const QString& _description, const QString& _currency_iso,
                            const QLocale& _language)
{
  QDateTime date_time   = QDateTime::currentDateTime();
  QString   folder_name = "rentalis_db_" + date_time.toString("yyyy-MM-dd_HH-mm-ss") + "_" + _name;
  QString   file_name   = folder_name + ".sqlite";
  QString   db_path     = SAVE_PATH() + "/" + folder_name;

  QDir().mkpath(file_name);
  QDir().mkpath(db_path + "/config.ini");

  QDir().mkpath(db_path + "/attachments/damages");
  QDir().mkpath(db_path + "/attachments/invoices");
  QDir().mkpath(db_path + "/attachments/landlords");
  QDir().mkpath(db_path + "/attachments/lease_agreements");
  QDir().mkpath(db_path + "/attachments/maintenances");
  QDir().mkpath(db_path + "/attachments/properties");
  QDir().mkpath(db_path + "/attachments/receipts");
  QDir().mkpath(db_path + "/attachments/tenants");


  if (!QFile::exists(db_path)) {
    // template copy
    QFile file(":/database/database_template.sqlite");
    if (!file.open(QIODevice::ReadOnly)) {
      qCritical() << "Impossible to open the template";
      return false;
    }
    QFile out(db_path);
    if (!out.open(QIODevice::WriteOnly)) {
      qCritical() << "Impossible to create the file :" << db_path;
      return false;
    }
    out.write(file.readAll());
    out.close();
    file.close();
  }


  settings = DB_Settings(db_path);
  if (!settings.is_valid()) {
    qCritical() << "Impossible to create settings";
    return false;
  }
  settings.set_name(_name);
  settings.set_description(_description);
  settings.set_currency(_currency_iso);
  settings.set_language(_language);
  settings.set_save_time(date_time); // avoid potential date time difference
  settings.save("db_creation");      // save settings

  // create SQLite connexion
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", settings.get_uuid());
  db.setDatabaseName(file_name);
  if (!db.open()) {
    qCritical() << "Impossible to open the base:" << file_name;
    return false;
  }

  QSqlQuery pragma(db);
  pragma.exec("PRAGMA journal_mode=WAL;");
  pragma.exec("PRAGMA busy_timeout=5000;");

  database_valid = true;

  return true;
}

QString Database::get_database_path() const
{
  return SAVE_PATH() + "/" + get_folder_name();
}

QString Database::get_file_path() const
{
  return get_database_path() + "/" + get_file_name();
}

QString Database::get_settings_path() const
{
  return get_database_path() + "/" + "config.ini";
}

bool Database::check_db_integrity() const
{
  const QString connName = settings.get_uuid();

  if (!QSqlDatabase::contains(connName)) {
    return false;
  }
  auto db = QSqlDatabase::database(connName);

  QStringList tables = db.tables();
}

QString Database::get_folder_name() const
{
  return "rentalis_db_" + settings.get_save_time().toString("yyyy-MM-dd_HH-mm-ss") + "_"
         + sanitize_fileName(settings.get_name());
}

QString Database::get_file_name() const
{
  return get_folder_name() + ".sqlite";
}

QSqlDatabase Database::get_sql_db()
{
  const QString connName = settings.get_uuid();

  if (QSqlDatabase::contains(connName)) {
    return QSqlDatabase::database(connName);
  }

  return QSqlDatabase();
}

int Database::last_insert_id()
{
  QSqlQuery query(get_sql_db());
  if (!query.exec("SELECT last_insert_rowid();")) {
    qDebug() << "Error last_insert_rowid:" << query.lastError().text();
    return -1;
  }
  if (query.next()) {
    return query.value(0).toInt();
  } else {
    qDebug() << "None value returned by last_insert_rowid()";
    return -1;
  }
}


// SELECT month, rent_id
std::vector<std::tuple<int, int>> Database::find_rents_on_year(int year, int property_id)
{
  std::vector<std::tuple<int, int>> vec;
  vec.reserve(12);

  QSqlQuery query(get_sql_db());
  query.prepare(R"(
            SELECT
                strftime("%m", rents.date) as month,
                rents.rent_id as rent_id
            FROM rents
            INNER JOIN properties
                ON properties.property_id = rents.property_id
            WHERE properties.property_id = :property_id
            AND rents.date >= :start_date
            AND rents.date < :end_date
            ORDER BY month
        )");

  QDate start_date(year, 1, 1);
  QDate end_date = start_date.addYears(1);

  query.bindValue(":start_date", start_date.toString(Qt::ISODate));
  query.bindValue(":end_date", end_date.toString(Qt::ISODate));

  // name of the property only
  query.bindValue(":property_id", property_id);

  if (!query.exec()) {
    qDebug() << "Error SELECT rents:" << query.lastError().text();
    return vec;
  }

  while (query.next()) {
    vec.push_back({query.value("month").toInt(), query.value("rent_id").toInt()});
  }

  return vec;
}

int Database::is_rent_exist(EMonth month, int year, int property_id, bool msg)
{
  QSqlQuery query(get_sql_db());
  query.prepare(R"(
            SELECT rent_id
            FROM rents
            WHERE property_id = :property_id
            AND date >= :start_date
            AND date < :end_date;
        )");
  QDate start(year, static_cast<int>(month), 1);
  QDate end = start.addMonths(1);

  query.bindValue(":start_date", start.toString(Qt::ISODate));
  query.bindValue(":end_date", end.toString(Qt::ISODate));

  query.bindValue(":property_id", property_id);

  if (query.exec() && query.next()) {
    return query.value("rent_id").toInt();
  }
  return 0;
}

// SUM of rent, housing_aid, charge, household_waste
std::tuple<float, float, float, float> Database::total_year_rent_sum(int year, int property_id)
{
  QSqlQuery query(get_sql_db());
  query.prepare(R"(
            SELECT
                SUM(rent) as rent,
                SUM(housing_aid) as housing_aid,
                SUM(charge) as charge,
                SUM(household_waste) as household_waste
            FROM rents
            WHERE property_id = :id
            AND date >= :start_date
            AND date < :end_date;
        )");

  QDate start_date(year, 1, 1);
  QDate end_date = start_date.addYears(1);

  query.bindValue(":id", property_id);
  query.bindValue(":start_date", start_date.toString(Qt::ISODate));
  query.bindValue(":end_date", end_date.toString(Qt::ISODate));

  if (query_check(&query, "total year rent sum") && query.next()) {
    return {query.value("rent").toFloat(), query.value("housing_aid").toFloat(), query.value("charge").toFloat(),
            query.value("household_waste").toFloat()};
  }

  return {};
}

// find the linked rent and property  according to the receipts.tenant_id and receipts.start_date
// SELECT rent_id, rent, name, address
std::optional<QSqlQuery> Database::find_property_rents_from_tenant_date(int tenant_id, const QDate& start_date,
                                                                        const QDate& end_date)
{
  QSqlQuery query(get_sql_db());
  query.prepare(R"(
            SELECT rents.rent_id as rent_id, rents.rent as rent, properties.name as name, properties.address as address
            FROM properties
            INNER JOIN rents
                ON rents.property_id = properties.property_id
            WHERE rents.tenant_id = :tenant_id
            AND rents.date >= :start_date
            AND rents.date < :end_date
            ORDER BY rents.date DESC;
        )");

  QDate _start_date(start_date.year(), start_date.month(), 1);
  QDate _end_date(start_date.year(), start_date.month(), 1);

  query.bindValue(":tenant_id", tenant_id);
  query.bindValue(":start_date", _start_date.toString(Qt::ISODate));
  query.bindValue(":end_date", _end_date.toString(Qt::ISODate));

  if (!query.exec()) return std::nullopt;

  return query;
}

bool Database::tenant_used(int tenant_id)
{
  QSqlQuery query(get_sql_db());
  query.prepare(R"(
            SELECT
                EXISTS(SELECT 1 FROM rents WHERE tenant_id = :tenant_id)
                OR EXISTS(SELECT 1 FROM receipts WHERE tenant_id = :tenant_id)
                OR EXISTS(SELECT 1 FROM invoices WHERE tenant_id = :tenant_id)
                AS used;
        )");
  query.bindValue(":tenant_id", tenant_id);

  query_check(&query, "EXISTS on tenant_id: " + QString::number(tenant_id));
  query.next();

  return query.value("used").toInt() > 0;
}

bool Database::property_used(int property_id)
{
  QSqlQuery query(get_sql_db());
  query.prepare(R"(
        SELECT
            EXISTS(SELECT 1 FROM rents WHERE property_id = :property_id)
            AS used;
    )");
  query.bindValue(":property_id", property_id);

  query_check(&query, "EXISTS on property_id: " + QString::number(property_id));
  query.next();

  return query.value("used").toInt() > 0;
}


bool Database::is_valid_column(ETable table, const QString& column)
{
  QSqlQuery query(get_sql_db());
  QString   table_name = ETable_to_str(table);

  QString q_text = QString(R"(
        SELECT COUNT(*) AS column_exists
        FROM pragma_table_info('%1')
        WHERE name = :column;
    )")
                       .arg(table_name);

  query.prepare(q_text);
  query.bindValue(":column", column);

  if (query.exec() && query.next()) return query.value("column_exists").toInt() == 1;

  QMessageBox::critical(
      nullptr, QObject::tr("Invalid Column"),
      QObject::tr("Invalid column name '%1'.\nThe column dosen't exist in the table %2 in the database.")
          .arg(column, ETable_to_str(table)));
  qCritical() << "Invalid column name '" << column << "' for table '" << table_name << "'";
  return false;
}

QString Database::primaryKey(ETable table)
{
  QSqlIndex pk = get_sql_db().primaryIndex(ETable_to_str(table));

  if (pk.isEmpty()) {
    qCritical() << "The table '" << ETable_to_str(table) << "' don't have any primary key";
    return "";
  }

  return pk.fieldName(0);
}


int Database::insert_record(ETable table, bool ask_msg, const QString& ask_info)
{
  if (ask_msg) {
    auto result = QMessageBox::question(
        nullptr, QObject::tr("%1 Creation").arg(ETable_to_str(table)),
        QObject::tr("Do you really want to create the record (%1) ?\n%2").arg(ask_info, TXT::WARNING_OPERATION),
        QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) return -1;
  }

  QSqlQuery query(get_sql_db());

  QString sql = QString("INSERT INTO %1 DEFAULT VALUES").arg(ETable_to_str(table));

  query.prepare(sql);

  if (!query.exec()) {
    QMessageBox::critical(nullptr, QObject::tr("Creation Failed"),
                          QObject::tr("The creation of the record at table %1 failed.\nSQL Error:\n%2")
                              .arg(ETable_to_str(table), query.lastError().text()));
    qDebug() << query.lastError().text();
    return -1;
  }

  signal_db_updated();
  return query.lastInsertId().toInt();
}


bool Database::update_record(ETable table, int record_id, const QString& column, const QVariant& value, bool ask_msg)
{
  if (!is_valid_column(table, column)) return false;

  if (ask_msg) {
    auto rec = find_record(table, record_id);
    if (!rec->next()) {
      QMessageBox::critical(nullptr, QObject::tr("Update Failed"),
                            QObject::tr("The update of the record[%1] at table %2 failed. The record dosen't exist.")
                                .arg(QString::number(record_id), ETable_to_str(table)));
      qDebug() << "Record of " << ETable_to_str(table) << "[" << record_id << "] doesn't exist";
      return false;
    }

    auto old_val = rec->value(column);

    auto result = QMessageBox::question(
        nullptr, QObject::tr("%1 Update").arg(ETable_to_str(table)),
        QObject::tr("Do you really want to update the record[%1] for %2 from %3 to %4 ?\n%2")
            .arg(QString::number(record_id), column, old_val.toString(), value.toString(), TXT::WARNING_OPERATION),
        QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) return false;
  }

  QSqlQuery query(get_sql_db());

  QString sql =
      QString("UPDATE %1 SET %2 = :value WHERE %3 = :id").arg(ETable_to_str(table), column, primaryKey(table));

  query.prepare(sql);
  query.bindValue(":value", value);
  query.bindValue(":id", record_id);

  if (!query.exec()) {
    QMessageBox::critical(nullptr, QObject::tr("Update Failed"),
                          QObject::tr("The update of the record at table %1 failed.\nSQL Error:\n%2")
                              .arg(ETable_to_str(table), query.lastError().text()));
    qCritical() << query.lastError().text();
    return false;
  }

  signal_db_updated();
  return query.numRowsAffected() > 0;
}

bool Database::delete_file(ETable table, const QString& uuid, bool warning_msg, const QString& w_info)
{
  Record_File f(table, uuid);

  if (!f.is_loaded()) {
    qWarning() << "Record file " << ETable_to_str(table) << " uuid: " << uuid << " is invalid!";
    return false;
  }

  QString target_path = f.get_file_path();

  if (warning_msg) {
    auto result = QMessageBox::warning(nullptr, QObject::tr("%1 File Deletion").arg(ETable_to_str(table)),
                                       QObject::tr("Do you really want to delete the file %1 at %2 ?\n%3")
                                           .arg(f.get_info_name(), target_path, TXT::WARNING_OPERATION),
                                       QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) return false;
  }

  if (QFile::exists(target_path)) {
    if (!QFile::remove(target_path)) {
      qCritical() << "Deletion of " << target_path << " failed !";
    }
  } else {
    qWarning() << "File at " << target_path << " not found, deletion impossible!";
  }

  QSqlQuery query(get_sql_db());

  QString sql = QString("DELETE FROM %1 WHERE uuid = :uuid").arg(ETable_to_str(table));

  query.prepare(sql);
  query.bindValue(":uuid", uuid);

  if (!query.exec()) {
    QMessageBox::critical(nullptr, QObject::tr("Deletion Failed"),
                          QObject::tr("The deletion of the record at table %1 failed.\nSQL Error:\n%2")
                              .arg(ETable_to_str(table), query.lastError().text()));
    qCritical() << query.lastError().text();
    return false;
  }

  signal_db_updated();
  return query.numRowsAffected() > 0;
}


bool Database::delete_record(ETable table, int record_id, bool warning_msg, const QString& w_info)
{
  if (warning_msg) {
    auto result = QMessageBox::warning(
        nullptr, QObject::tr("%1 Deletion").arg(ETable_to_str(table)),
        QObject::tr("Do you really want to delete the record (%1) ?\n%2").arg(w_info, TXT::WARNING_OPERATION),
        QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) return false;
  }

  QSqlQuery query(get_sql_db());

  QString sql = QString("DELETE FROM %1 WHERE %2 = :id").arg(ETable_to_str(table), primaryKey(table));

  query.prepare(sql);
  query.bindValue(":id", record_id);

  if (!query.exec()) {
    QMessageBox::critical(nullptr, QObject::tr("Deletion Failed"),
                          QObject::tr("The deletion of the record at table %1 failed.\nSQL Error:\n%2")
                              .arg(ETable_to_str(table), query.lastError().text()));
    qCritical() << query.lastError().text();
    return false;
  }

  signal_db_updated();
  return query.numRowsAffected() > 0;
}

std::optional<QSqlQuery> Database::find_record(ETable table, int record_id)
{
  QSqlQuery query(get_sql_db());

  QString sql = QString("SELECT * FROM %1 WHERE %2 = :id").arg(ETable_to_str(table), primaryKey(table));

  query.prepare(sql);
  query.bindValue(":id", record_id);

  if (!query.exec()) {
    qCritical() << query.lastError().text();
    return std::nullopt;
  }

  return query;
}

std::optional<QSqlQuery> Database::all_records(ETable table)
{
  QSqlQuery query(get_sql_db());

  QString sql = QString("SELECT * FROM %1").arg(ETable_to_str(table));

  if (!query.exec(sql)) {
    qDebug() << query.lastError().text();
    return std::nullopt;
  }

  return query;
}

int Database::get_record_id(ETable table, const QString& col_name, const QVariant& key)
{
  if (!is_valid_column(table, col_name)) return -1;

  QSqlQuery query(get_sql_db());

  QString sql = QString("SELECT * FROM %1 WHERE %2 = :key LIMIT 1").arg(ETable_to_str(table), col_name);

  query.prepare(sql);
  query.bindValue(":key", key);

  if (!query.exec()) {
    qCritical() << query.lastError().text();
    return -1;
  }

  if (!query.next()) return -1;

  // assume the first column is the id
  return query.value(primaryKey(table)).toInt();
}

// final_path = save_path + database_name + table_category
QString Database::get_table_file_path(ETable table)
{
  if (!ETable_is_files(table)) return QString{};

  QString parent_path = DB_MANAGER.get_db()->get_database_path() + "/attachments";
  QString table_folder;

  switch (table) {
  case ETable::properties_files:       table_folder = "properties";
  case ETable::invoices_files:         table_folder = "invoices";
  case ETable::receipts_files:         table_folder = "receipts";
  case ETable::tenants_files:          table_folder = "tenants";
  case ETable::landlords_files:        table_folder = "landlords";
  case ETable::damages_files:          table_folder = "damages";
  case ETable::maintenances_files:     table_folder = "maintenances";
  case ETable::lease_agreements_files: table_folder = "lease_agreements";
  default:                             return QString{};
  }

  return parent_path + "/" + table_folder;
}

QString Database::insert_file(ETable table, int target_record_id, const QString& source_path, const QString& extension,
                              const QString& info_name)
{
  if (!ETable_is_files(table)) {
    qCritical() << "table: " << ETable_to_str(table) << " is not a table files !";
    return QString{};
  }

  QString record_id_name = ETable_files_to_record_id_name(table);

  if (target_record_id < 1) {
    qCritical() << record_id_name << " : " << target_record_id << " is invalid !";
    return QString{};
  }

  if (!QFile::exists(source_path)) {
    qWarning() << "File at " << source_path << " dosen't exists !";
    return QString{};
  }

  QString file_uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);

  QString target_path = get_table_file_path(table) + "/" + file_uuid + extension;

  if (!QFile::copy(source_path, target_path)) {
    qWarning() << "The copy of file from " << source_path << " to " << target_path << " failed !";
    QString{};
  }

  QSqlQuery query(get_sql_db());
  QString   q_text =
      QString(
          "INSERT INTO %1 (%2, uuid, info_name, extension) VALUES (:target_record_id, :uuid, :info_name, :extension)")
          .arg(ETable_to_str(table), target_record_id);
  query.prepare(q_text);
  query.bindValue(":target_record_id", target_record_id);
  query.bindValue(":uuid", file_uuid);
  query.bindValue(":info_name", info_name);
  query.bindValue(":extension", extension);

  if (!query.exec()) {
    QMessageBox::critical(nullptr, tr("Insert File Query Failure"),
                          tr("The insert query for the file failed !\n%1").arg(query.lastError().text()));
    return QString{};
  }

  return file_uuid;
}

bool Database::update_file_info_name(ETable table, const QString& _uuid, const QString& info_name)
{
  if (!ETable_is_files(table)) {
    qCritical() << "table: " << ETable_to_str(table) << " is not a table files !";
    return false;
  }

  QString table_name = ETable_to_str(table);

  QSqlQuery query(get_sql_db());
  QString   q_text = QString("UPDATE %1 SET info_name = :info_name WHERE uuid = :uuid;").arg(table_name);
  query.prepare(q_text);
  query.bindValue(":uuid", _uuid);
  query.bindValue(":info_name", info_name);

  if (!query.exec()) {
    QMessageBox::critical(nullptr, tr("Update File Query Failure"),
                          tr("The update query for the file failed !\n%1").arg(query.lastError().text()));
    return false;
  }

  return true;
}
