#include "record_file.h"

#include "base.h"

#include <QFile>
#include <QUuid>

void Record_File::ensure_loaded()
{
  if (loaded || dirty) return;

  if (!DB_MANAGER.is_valid()) return;

  if (!ETable_is_files(table)) return;

  QSqlQuery query(DB_MANAGER.get_sql_db());
  QString   q_text = QString("SELECT * FROM %1 WHERE uuid = :uuid LIMIT 1;").arg(ETable_to_str(table));
  query.prepare(q_text);
  query.bindValue(":uuid", uuid);

  if (!query.exec() || !query.next()) {
    qDebug() << "File at table: " << ETable_to_str(table) << ", uuid: " << uuid << " is invalid !";
    dirty  = false;
    loaded = false;
    return;
  }

  record_id = query.value(ETable_files_to_record_id_name(table)).toInt();
  info_name = query.value("info_name").toString();
  extension = query.value("extension").toString();

  dirty  = false;
  loaded = true;
}

QString Record_File::get_file_path() const
{
  return DB_MANAGER.get_db()->get_table_file_path(table) + "/" + uuid + "." + extension;
}

bool Record_File::delete_record(bool w_msg)
{
  if (!loaded && !dirty) {
    qWarning() << "Cannot delete: record_file not loaded.";
    return false;
  }

  if (!DB_MANAGER.is_valid()) return false;

  if (!DB_MANAGER.get_db()->delete_file(table, uuid, w_msg, "")) {
    return false;
  }

  // reset
  record_id = -1;
  uuid.clear();
  info_name.clear();
  extension.clear();
  new_file.clear();

  loaded = false;
  dirty  = false;
  return true;
}

QString Record_File::insert_record()
{
  if (loaded) {
    qWarning() << "Cannot insert record: the file is already loaded.";
    return QString{};
  }

  uuid = DB_MANAGER.get_db()->insert_file(table, record_id, new_file, extension, info_name);

  return uuid;
}

bool Record_File::update_record()
{
  if (dirty) return -1;

  if (!DB_MANAGER.get_db()->update_file_info_name(table, uuid, info_name)) {
    qCritical() << "Update failed for record_file uuid : " << uuid;
    return false;
  }

  loaded = true;
  dirty  = false;
  return true;
}
