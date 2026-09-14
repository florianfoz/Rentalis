#ifndef DATABASE_H
#define DATABASE_H

#include "forward.h"

#include <QDate>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>


const QMap<ETable, QString> DB_SCHEMA = {
    {ETable::Property, {}}
};


class Database : public QObject
{
  Q_OBJECT

public:
  explicit Database(QObject* parent = nullptr);
  ~Database() = default;

  [[nodiscard]] static Database* open_database(const QString& db_path);
  [[nodiscard]] static Database* new_database(const QString& name, const QString& _description,
                                              const QString& _currency_iso, const QLocale& _language);

  [[nodiscard]] static Database& invalid()
  {
    static auto db     = Database();
    db._database_valid = false;
    return db;
  }

  // getters
  [[nodiscard]] QString            file_name() const;
  [[nodiscard]] QSqlDatabase       sql();
  [[nodiscard]] Database_Manifest& manifest()
  {
    return *_manifest;
  }

  [[nodiscard]] bool is_valid() const
  {
    return _database_valid;
  }

  explicit operator bool() const
  {
    return is_valid();
  }

  // database file manager
  void               save(const QString& _save_method = {});
  void               remove();
  [[nodiscard]] bool update_metadata(const QString& _save_method = {});

  // database queries
  [[nodiscard]] int                         last_insert_id();
  [[nodiscard]] QList<std::tuple<int, int>> find_rents_on_year(int year, int property_id);
  [[nodiscard]] int                         is_rent_exist(EMonth month, int year, int property_id, bool msg = false);
  [[nodiscard]] std::tuple<float, float, float, float> total_year_rent_sum(int year, int property_id);
  [[nodiscard]] std::optional<QSqlQuery> find_property_rents_from_tenant_date(int tenant_id, const QDate& start_date,
                                                                              const QDate& end_date);
  [[nodiscard]] bool                     tenant_used(int tenant_id);
  [[nodiscard]] bool                     property_used(int property_id);
  [[nodiscard]] bool                     is_valid_column(ETable table, const QString& column);
  [[nodiscard]] QString                  primaryKey(ETable table);

  [[nodiscard]] bool                     contains_record(ETable table, qsizetype record_id);
  [[nodiscard]] std::optional<QSqlQuery> find_record(ETable table, qsizetype record_id);
  [[nodiscard]] std::optional<QSqlQuery> all_records(ETable table);
  [[nodiscard]] std::optional<QSqlQuery> all_records_id(ETable table);
  [[nodiscard]] int                      record_id(ETable table, const QString& col_name, const QVariant& key);
  [[nodiscard]] QString                  table_file_path(ETable table);

  // record operation
  // returns record id
  [[nodiscard]] qsizetype insert_record(ETable table, bool ask_msg = true, const QString& ask_info = {});
  bool                    replace_record(ETable table, qsizetype record_id, const QSqlQuery& query);
  [[nodiscard]] bool      save_record(ETable table, qsizetype record_id, const QString& column, const QVariant& value,
                                      bool ask_msg = false);
  [[nodiscard]] bool      delete_record(ETable table, qsizetype record_id, bool warning_msg = true,
                                        const QString& w_info = "");

  [[nodiscard]] QString folder_name() const;
  [[nodiscard]] QString database_path() const;
  [[nodiscard]] QString file_path() const;
  [[nodiscard]] QString manifest_path() const;

  [[nodiscard]] bool check_db_integrity() const;

private:
  Database_Manifest* _manifest;
  bool               _database_valid = false;

public:
  static bool query_check(QSqlQuery* query, const QString& tag);


signals:
  void signal_db_updated();
};

#endif // DATABASE_H
