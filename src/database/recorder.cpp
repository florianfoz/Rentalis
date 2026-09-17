#include "database/recorder.h"

#include "base.h"
#include "database/manager.h"

Database_Recorder Database_Recorder::create(const QString& sqlite_path, const QString& uuid)
{
  Database_Recorder out;
  out.sql = QSqlDatabase::addDatabase("QSQLITE", uuid);
  out.sql.setDatabaseName(sqlite_path);
  if (!out.sql.open()) {
    qCritical() << "Impossible to open database:" << out.sql.databaseName();
    qCritical() << "Error:" << out.sql.lastError().text();
    return {};
  }

  QSqlQuery pragma(out.sql);
  pragma.exec("PRAGMA journal_mode=WAL;");
  pragma.exec("PRAGMA busy_timeout=5000;");

  return out;
}


Database_Recorder Database_Recorder::load(const QString& sqlite_path, const QString& uuid)
{
  Database_Recorder out;
  out.sql = QSqlDatabase::addDatabase("QSQLITE", uuid);
  out.sql.setDatabaseName(sqlite_path);
  if (!out.sql.open()) {
    qCritical() << "Impossible to open database:" << out.sql.databaseName();
    qCritical() << "Error:" << out.sql.lastError().text();
    return {};
  }

  return out;
}
void Database_Recorder::unload()
{
  sql.close();
  QSqlDatabase::removeDatabase(sql.connectionName());
}


bool Database_Recorder::query_check(QSqlQuery* query, const QString& tag)
{
  if (!query->exec()) {
    qDebug() << "Error on query exec: " << tag << ": " << query->lastError();
    return false;
  }
  return true;
}


qsizetype Database_Recorder::last_insert_id()
{
  QSqlQuery query(sql);
  if (!query.exec("SELECT last_insert_rowid();")) {
    qDebug() << "Error last_insert_rowid:" << query.lastError().text();
    return INVALID_ID;
  }
  if (query.next()) return query.value(0).toInt();

  qDebug() << "None value returned by last_insert_rowid()";
  return INVALID_ID;
}


bool Database_Recorder::is_valid_column(ETable table, const QString& column)
{
  QSqlQuery query(sql);
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

qsizetype Database_Recorder::insert_record(ETable table, bool ask_msg, const QString& ask_info)
{
  if (ask_msg) {
    auto result = QMessageBox::question(
        nullptr, QObject::tr("%1 Creation").arg(ETable_to_str(table)),
        QObject::tr("Do you really want to create the record (%1) ?\n%2").arg(ask_info, TXT::WARNING_OPERATION),
        QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) return INVALID_ID;
  }

  QSqlQuery query(sql);

  auto sql = QString("INSERT INTO %1 DEFAULT VALUES").arg(ETable_to_str(table));

  query.prepare(sql);

  if (!query.exec()) {
    QMessageBox::critical(nullptr, QObject::tr("Creation Failed"),
                          QObject::tr("The creation of the record at table %1 failed.\nSQL Error:\n%2")
                              .arg(ETable_to_str(table), query.lastError().text()));
    qDebug() << query.lastError().text();
    return INVALID_ID;
  }

  emit Database_Manager::instance().signal_db_updated(table);
  return query.lastInsertId().toInt();
}

bool Database_Recorder::replace_record(ETable table, qsizetype record_id, const QSqlQuery& query)
{
  if (!contains_record(table, record_id)) {
    qWarning() << "The record " << record_id << " at " << ETable_to_str(table) << " dosen't exists.";
    return false;
  }

  emit Database_Manager::instance().signal_db_updated(table);
  return true;
}


bool Database_Recorder::save_record(ETable table, qsizetype record_id, const QString& column, const QVariant& value,
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

  QSqlQuery query(sql);

  auto sql = QString("UPDATE %1 SET %2 = :value WHERE id = :id").arg(ETable_to_str(table), column);

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

  emit Database_Manager::instance().signal_db_updated(table);
  return query.numRowsAffected() > 0;
}

bool Database_Recorder::delete_record(ETable table, qsizetype record_id, bool warning_msg, const QString& w_info)
{
  if (warning_msg) {
    auto result = QMessageBox::warning(
        nullptr, QObject::tr("%1 Deletion").arg(ETable_to_str(table)),
        QObject::tr("Do you really want to delete the record (%1) ?\n%2").arg(w_info, TXT::WARNING_OPERATION),
        QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) return false;
  }

  QSqlQuery query(sql);

  auto sql = QString("DELETE FROM %1 WHERE id = :id").arg(ETable_to_str(table));

  query.prepare(sql);
  query.bindValue(":id", record_id);

  if (!query.exec()) {
    QMessageBox::critical(nullptr, QObject::tr("Deletion Failed"),
                          QObject::tr("The deletion of the record at table %1 failed.\nSQL Error:\n%2")
                              .arg(ETable_to_str(table), query.lastError().text()));
    qCritical() << query.lastError().text();
    return false;
  }

  emit Database_Manager::instance().signal_db_updated(table);
  return query.numRowsAffected() > 0;
}

bool Database_Recorder::contains_record(ETable table, qsizetype record_id)
{
  QSqlQuery query(sql);

  const auto sql = QString("SELECT EXISTS(SELECT 1 FROM %1 WHERE id = :id)").arg(ETable_to_str(table));

  query.prepare(sql);
  query.bindValue(":id", record_id);

  if (!query.exec()) {
    qCritical() << query.lastError().text();
    return false;
  }

  return query.next() && query.value(0).toBool();
}

std::optional<QSqlQuery> Database_Recorder::find_record(ETable table, qsizetype record_id)
{
  QSqlQuery query(sql);

  auto sql = QString("SELECT * FROM %1 WHERE id = :id").arg(ETable_to_str(table));

  query.prepare(sql);
  query.bindValue(":id", record_id);

  if (!query.exec()) {
    qCritical() << query.lastError().text();
    return std::nullopt;
  }

  return query;
}

std::optional<QSqlQuery> Database_Recorder::all_records(ETable table)
{
  QSqlQuery query(sql);

  auto sql = QString("SELECT * FROM %1").arg(ETable_to_str(table));

  if (!query.exec(sql)) {
    qDebug() << query.lastError().text();
    return std::nullopt;
  }

  return query;
}

std::optional<QSqlQuery> Database_Recorder::all_records_id(ETable table)
{
  QSqlQuery query(sql);

  auto sql = QString("SELECT id FROM %1").arg(ETable_to_str(table));

  if (!query.exec(sql)) {
    qDebug() << query.lastError().text();
    return std::nullopt;
  }

  return query;
}


qsizetype Database_Recorder::record_id(ETable table, const QString& col_name, const QVariant& key)
{
  if (!is_valid_column(table, col_name)) return INVALID_ID;

  QSqlQuery query(sql);

  auto sql = QString("SELECT * FROM %1 WHERE %2 = :key LIMIT 1").arg(ETable_to_str(table), col_name);

  query.prepare(sql);
  query.bindValue(":key", key);

  if (!query.exec()) {
    qCritical() << query.lastError().text();
    return INVALID_ID;
  }

  if (!query.next()) return INVALID_ID;

  // assume the first column is the id
  return query.value("id").toInt();
}
