#include "database/database.h"

#include "base.h"
#include "database/manager.h"
#include "database/manifest.h"

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

void Database::save(const QString& _save_method)
{
  if (!_database_valid) {
    qWarning() << "Cannot save an invalid database.";
    return;
  }

  const QDateTime save_time = QDateTime::currentDateTime();

  _manifest->last_save_time = save_time;
  _manifest->save(_save_method);

  emit signal_db_updated();
}

bool Database::update_metadata(const QString& _save_method)
{
  if (!_database_valid) {
    qWarning() << "Cannot update metadata of an invalid database.";
    return false;
  }

  const QString old_folder_path = database_path();

  if (!QDir(old_folder_path).exists()) {
    qCritical() << "Database folder does not exist:" << old_folder_path;
    return false;
  }

  const QString old_file_path = file_path();

  // Save current metadata first.
  _manifest->last_save_time = QDateTime::currentDateTime();
  _manifest->save(_save_method);

  const QString new_folder_path = database_path();
  const QString new_file_path   = file_path();

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
  const QString connection_name = _manifest->uuid;

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
      _database_valid = false;
      qCritical() << "Cannot reopen database after failed metadata update:" << db.lastError().text();
    }

    return false;
  }

  // Recreate the SQLite connection with the new file path.
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connection_name);
  db.setDatabaseName(new_file_path);

  if (!db.open()) {
    qCritical() << "Database folder was renamed, but SQLite cannot reopen:" << db.lastError().text();

    _database_valid = false;
    return false;
  }

  // Restore SQLite configuration.
  QSqlQuery pragma(db);
  pragma.exec("PRAGMA journal_mode=WAL;");
  pragma.exec("PRAGMA busy_timeout=5000;");

  _database_valid = true;

  emit signal_db_updated();

  return true;
}

void Database::remove()
{
  if (!_database_valid) {
    qWarning() << "Cannot remove an invalid database.";
    return;
  }

  const QString database_path = DATABASE_PATH();

  if (!QDir(database_path).exists()) {
    qWarning() << "Database folder does not exist:" << database_path;
    _database_valid = false;
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

  const QString connection_name = _manifest->uuid;

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

  _database_valid = false;

  // emit signal_db_updated();
}

Database* Database::open_database(const QString& db_path)
{
  if (db_path.isEmpty()) return nullptr;

  // if no database existing : build a new database by coping the template
  if (!QFile::exists(db_path)) {
    QMessageBox::warning(nullptr, tr("Open Database Error"),
                         tr("Impossible to open the database at %1\nThe database dosen't exists.").arg(db_path));
    return nullptr;
  }

  auto* db                 = new Database();
  // test manifest
  auto  manifest_file_path = db_path + "/manifest.conf";
  db->_manifest            = Database_Manifest::load(manifest_file_path);
  if (!db->_manifest) {
    qCritical() << "Cannot opening the database manifest file at " << manifest_file_path << " in Qmanifest";
    delete db;
    return nullptr;
  }
  QSqlDatabase::addDatabase("QSQLITE", db->_manifest->uuid);

  db->_database_valid = true;
  return db;
}

Database* Database::new_database(const QString& _name, const QString& _description, const QString& _currency_iso,
                                 const QLocale& _language)
{
  QDateTime date_time   = QDateTime::currentDateTime();
  QString   folder_name = "rentalis_db_" + date_time.toString("yyyy-MM-dd_HH-mm-ss") + "_" + _name;
  QString   file_name   = folder_name + ".sqlite";
  QString   db_path     = SAVE_PATH() + "/" + folder_name;
  QString   db_file     = db_path + "/" + file_name;

  QDir().mkpath(db_path);

  QDir().mkpath(db_path + "/attachments/damages");
  QDir().mkpath(db_path + "/attachments/invoices");
  QDir().mkpath(db_path + "/attachments/landlords");
  QDir().mkpath(db_path + "/attachments/lease_agreements");
  QDir().mkpath(db_path + "/attachments/maintenances");
  QDir().mkpath(db_path + "/attachments/properties");
  QDir().mkpath(db_path + "/attachments/receipts");
  QDir().mkpath(db_path + "/attachments/tenants");


  if (!QFile::exists(db_file)) {
    // template copy
    QFile ftemplate(":/database/database_template.sqlite");
    if (!ftemplate.open(QIODevice::ReadOnly)) {
      qCritical() << "Impossible to open the template";
      return nullptr;
    }
    QFile out(db_file);
    if (!out.open(QIODevice::WriteOnly)) {
      qCritical() << "Impossible to create the file :" << db_file;
      return nullptr;
    }
    out.write(ftemplate.readAll());
    out.close();
    ftemplate.close();
  }

  auto manifest_path = db_path + "/manifest.conf";

  auto* db = new Database();

  // init manifest config default values
  db->_manifest = Database_Manifest::create(manifest_path, _name, _description);

  if (!db->_manifest->is_valid()) {
    qCritical() << "Impossible to create manifest";
    return nullptr;
  }
  db->_manifest->name           = _name;
  db->_manifest->description    = _description;
  db->_manifest->currency       = CurrencyInfo::from_iso(_currency_iso);
  db->_manifest->language       = _language;
  db->_manifest->last_save_time = date_time; // avoid potential date time difference
  db->_manifest->save("db_creation");        // save manifest

  // create SQLite connexion
  QSqlDatabase sql_db = QSqlDatabase::addDatabase("QSQLITE", db->_manifest->uuid);
  sql_db.setDatabaseName(file_name);
  if (!sql_db.open()) {
    qCritical() << "Impossible to open database:" << sql_db.databaseName();
    qCritical() << "Error:" << sql_db.lastError().text();
    return nullptr;
  }

  QSqlQuery pragma(sql_db);
  pragma.exec("PRAGMA journal_mode=WAL;");
  pragma.exec("PRAGMA busy_timeout=5000;");

  db->_database_valid = true;

  return db;
}

QString Database::database_path() const
{
  return SAVE_PATH() + "/" + folder_name();
}

QString Database::file_path() const
{
  return database_path() + "/" + file_name();
}

QString Database::manifest_path() const
{
  return database_path() + "/manifest.conf";
}

bool Database::check_db_integrity() const
{
  const QString connName = _manifest->uuid;

  if (!QSqlDatabase::contains(connName)) {
    return false;
  }
  auto db = QSqlDatabase::database(connName);

  QStringList tables = db.tables();
}

QString Database::folder_name() const
{
  return "rentalis_db_" + _manifest->last_save_time.toString("yyyy-MM-dd_HH-mm-ss") + "_"
         + sanitize_fileName(_manifest->name);
}

QString Database::file_name() const
{
  return folder_name() + ".sqlite";
}

QSqlDatabase Database::sql()
{
  const QString connName = _manifest->uuid;

  if (QSqlDatabase::contains(connName)) {
    return QSqlDatabase::database(connName);
  }

  return {};
}

int Database::last_insert_id()
{
  QSqlQuery query(sql());
  if (!query.exec("SELECT last_insert_rowid();")) {
    qDebug() << "Error last_insert_rowid:" << query.lastError().text();
    return -1;
  }
  if (query.next()) return query.value(0).toInt();

  qDebug() << "None value returned by last_insert_rowid()";
  return -1;
}


// SELECT month, rent_id
QList<std::tuple<int, int>> Database::find_rents_on_year(int year, int property_id)
{
  QList<std::tuple<int, int>> vec;
  vec.reserve(12);

  QSqlQuery query(sql());
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
  QSqlQuery query(sql());
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
  QSqlQuery query(sql());
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
  QSqlQuery query(sql());
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
  QSqlQuery query(sql());
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
  QSqlQuery query(sql());
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
  QSqlQuery query(sql());
  auto      table_name = ETable_to_str(table);

  auto q_text = QString(R"(
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
  QSqlIndex pk = sql().primaryIndex(ETable_to_str(table));

  if (pk.isEmpty()) {
    qCritical() << "The table '" << ETable_to_str(table) << "' don't have any primary key";
    return "";
  }

  return pk.fieldName(0);
}


qsizetype Database::insert_record(ETable table, bool ask_msg, const QString& ask_info)
{
  if (ask_msg) {
    auto result = QMessageBox::question(
        nullptr, QObject::tr("%1 Creation").arg(ETable_to_str(table)),
        QObject::tr("Do you really want to create the record (%1) ?\n%2").arg(ask_info, TXT::WARNING_OPERATION),
        QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) return -1;
  }

  QSqlQuery query(sql());

  auto sql = QString("INSERT INTO %1 DEFAULT VALUES").arg(ETable_to_str(table));

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

bool Database::replace_record(ETable table, qsizetype record_id, const QSqlQuery& query)
{
  if (!contains_record(table, record_id)) {
    qWarning() << "The record " << record_id << " at " << ETable_to_str(table) << " dosen't exists.";
    return false;
  }
}


bool Database::save_record(ETable table, qsizetype record_id, const QString& column, const QVariant& value,
                           bool ask_msg)
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

  QSqlQuery query(sql());

  auto sql = QString("UPDATE %1 SET %2 = :value WHERE %3 = :id").arg(ETable_to_str(table), column, primaryKey(table));

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

bool Database::delete_record(ETable table, qsizetype record_id, bool warning_msg, const QString& w_info)
{
  if (warning_msg) {
    auto result = QMessageBox::warning(
        nullptr, QObject::tr("%1 Deletion").arg(ETable_to_str(table)),
        QObject::tr("Do you really want to delete the record (%1) ?\n%2").arg(w_info, TXT::WARNING_OPERATION),
        QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) return false;
  }

  QSqlQuery query(sql());

  auto sql = QString("DELETE FROM %1 WHERE %2 = :id").arg(ETable_to_str(table), primaryKey(table));

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

bool Database::contains_record(ETable table, qsizetype record_id)
{
  QSqlQuery query(sql());

  const auto sql =
      QString("SELECT EXISTS(SELECT 1 FROM %1 WHERE %2 = :id)").arg(ETable_to_str(table), primaryKey(table));

  query.prepare(sql);
  query.bindValue(":id", record_id);

  if (!query.exec()) {
    qCritical() << query.lastError().text();
    return false;
  }

  return query.next() && query.value(0).toBool();
}

std::optional<QSqlQuery> Database::find_record(ETable table, qsizetype record_id)
{
  QSqlQuery query(sql());

  auto sql = QString("SELECT * FROM %1 WHERE %2 = :id").arg(ETable_to_str(table), primaryKey(table));

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
  QSqlQuery query(sql());

  auto sql = QString("SELECT * FROM %1").arg(ETable_to_str(table));

  if (!query.exec(sql)) {
    qDebug() << query.lastError().text();
    return std::nullopt;
  }

  return query;
}

std::optional<QSqlQuery> Database::all_records_id(ETable table)
{
  QSqlQuery query(sql());

  auto sql = QString("SELECT id FROM %1").arg(ETable_to_str(table));

  if (!query.exec(sql)) {
    qDebug() << query.lastError().text();
    return std::nullopt;
  }

  return query;
}


int Database::record_id(ETable table, const QString& col_name, const QVariant& key)
{
  if (!is_valid_column(table, col_name)) return -1;

  QSqlQuery query(sql());

  auto sql = QString("SELECT * FROM %1 WHERE %2 = :key LIMIT 1").arg(ETable_to_str(table), col_name);

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
