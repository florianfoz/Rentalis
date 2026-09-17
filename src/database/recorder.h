#ifndef RECORDER_H
#define RECORDER_H

#include "forward.h"


class Database_Recorder final
{
public:
  [[nodiscard]] static Database_Recorder create(const QString& sqlite_path, const QString& uuid);
  [[nodiscard]] static Database_Recorder load(const QString& sqlite_path, const QString& uuid);
  void                                   unload();

  [[nodiscard]] qsizetype last_insert_id();
  [[nodiscard]] bool      is_valid_column(ETable table, const QString& column);
  [[nodiscard]] bool      query_check(QSqlQuery* query, const QString& tag);

  [[nodiscard]] bool                     contains_record(ETable table, qsizetype record_id);
  [[nodiscard]] std::optional<QSqlQuery> find_record(ETable table, qsizetype record_id);
  [[nodiscard]] std::optional<QSqlQuery> all_records(ETable table);
  [[nodiscard]] std::optional<QSqlQuery> all_records_id(ETable table);
  [[nodiscard]] qsizetype                record_id(ETable table, const QString& col_name, const QVariant& key);
  [[nodiscard]] QString                  table_file_path(ETable table);

  [[nodiscard]] qsizetype insert_record(ETable table, bool ask_msg = true, const QString& ask_info = {});
  bool                    replace_record(ETable table, qsizetype record_id, const QSqlQuery& query);
  [[nodiscard]] bool      save_record(ETable table, qsizetype record_id, const QString& column, const QVariant& value,
                                      bool ask_msg = false);
  [[nodiscard]] bool      delete_record(ETable table, qsizetype record_id, bool warning_msg = true,
                                        const QString& w_info = "");

  QSqlDatabase sql;
};

#endif // RECORDER_H