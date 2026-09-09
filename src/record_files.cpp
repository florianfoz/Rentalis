#include "record_files.h"

#include "base.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <qsqlerror.h>

Record_Files::Record_Files(ETable _table, int _record_id)
  : table(_table)
  , record_id(_record_id)
{
  ensure_loaded();
}

void Record_Files::ensure_loaded()
{
  if (loaded) return;

  if (!DB_MANAGER.is_valid()) return;

  if (!ETable_is_files(table)) return;

  QString table_name     = ETable_to_str(table);
  QString record_id_name = ETable_files_to_record_id_name(table);

  QSqlQuery query(DB_MANAGER.get_sql_db());
  QString   q_text = QString("SELECT * FROM %1 WHERE %2 = :record_id;").arg(table_name, record_id_name);
  query.prepare(q_text);
  query.bindValue(":record_id", record_id);

  if (!query.exec()) {
    QMessageBox::critical(nullptr, QObject::tr("Record File SQL Error"),
                          QObject::tr("The search in the table %1 for the record_id : %2 failed !\nSQL Error:\n%3")
                              .arg(table_name, record_id_name, query.lastError().text()));
    return;
  }

  while (query.next()) {
    Record_File f(table, query.value("uuid").toString());
    files.push_back(std::move(f));
  }

  loaded = true;
}

bool Record_Files::delete_all_files()
{
  refresh_data();

  QString table_name     = ETable_to_str(table);
  QString record_id_name = ETable_files_to_record_id_name(table);

  auto result = QMessageBox::warning(nullptr, QObject::tr("Files Deletion Error"),
                                     QObject::tr("The files deletion of table %1 at %2 : %3 failed !")
                                         .arg(table_name, record_id_name, QString::number(record_id)));

  if (result == QMessageBox::Cancel) return false;

  for (auto& f : files) {
    f.delete_record(false);
  }

  loaded = false;
  files.clear();
  return true;
}

void Record_Files::refresh_data()
{
  files.clear();

  loaded = false;

  ensure_loaded();
}
