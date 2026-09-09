#ifndef DATABASE_H
#define DATABASE_H

#include "db_settings.h"
#include "record_files.h"
#include "settings.h"

#include <QDate>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>

enum class ETable {
  properties,
  properties_files,
  invoices,
  invoices_files,
  receipts,
  receipts_files,
  rents,
  tenants,
  tenants_files,
  landlords,
  landlords_files,
  landlord_property,
  damages,
  damages_files,
  maintenances,
  maintenances_files,
  lease_agreements,
  lease_agreements_files,
  property_room,
  property_feature,
};

constexpr ETable all_tables[] = {
    ETable::properties,
    ETable::properties_files,
    ETable::invoices,
    ETable::invoices_files,
    ETable::receipts,
    ETable::receipts_files,
    ETable::rents,
    ETable::tenants,
    ETable::tenants_files,
    ETable::landlords,
    ETable::landlords_files,
    ETable::landlord_property,
    ETable::damages,
    ETable::damages_files,
    ETable::maintenances,
    ETable::maintenances_files,
    ETable::lease_agreements,
    ETable::lease_agreements_files,
    ETable::property_room,
    ETable::property_feature,
};

const QMap<ETable, QString> DB_SCHEMA = {
    {ETable::properties, {}}
};

enum class EMonth;


inline QString ETable_to_str(ETable table)
{
  switch (table) {
  case ETable::properties:             return "properties";
  case ETable::properties_files:       return "properties_files";
  case ETable::invoices:               return "invoices";
  case ETable::invoices_files:         return "invoices_files";
  case ETable::receipts:               return "receipts";
  case ETable::receipts_files:         return "receipts_files";
  case ETable::rents:                  return "rents";
  case ETable::tenants:                return "tenants";
  case ETable::tenants_files:          return "tenants_files";
  case ETable::landlords:              return "landlords";
  case ETable::landlords_files:        return "landlords_files";
  case ETable::landlord_property:      return "landlord_property";
  case ETable::damages:                return "damages";
  case ETable::damages_files:          return "damages_files";
  case ETable::maintenances:           return "maintenances";
  case ETable::maintenances_files:     return "maintenances_files";
  case ETable::lease_agreements:       return "lease_agreements";
  case ETable::lease_agreements_files: return "lease_agreements_files";
  case ETable::property_room:          return "property_room";
  case ETable::property_feature:       return "property_feature";
  }
}

inline bool ETable_is_files(ETable table)
{
  switch (table) {
  case ETable::properties_files:
  case ETable::invoices_files:
  case ETable::receipts_files:
  case ETable::tenants_files:
  case ETable::landlords_files:
  case ETable::damages_files:
  case ETable::maintenances_files:
  case ETable::lease_agreements_files: return true;
  default:                             qCritical() << ETable_to_str(table) << " is not a valid database table files!"; return false;
  }
}


inline QString ETable_files_to_record_id_name(ETable table)
{
  if (!ETable_is_files(table)) return QString{};

  switch (table) {
  case ETable::properties_files:       return "property_id";
  case ETable::invoices_files:         return "invoice_id";
  case ETable::receipts_files:         return "receipt_id";
  case ETable::tenants_files:          return "tenant_id";
  case ETable::landlords_files:        return "landlord_id";
  case ETable::damages_files:          return "damage_id";
  case ETable::maintenances_files:     return "maintenance_id";
  case ETable::lease_agreements_files: return "lease_agreement_id";
  default:                             return QString{};
  }
}

class Database : public QObject
{
  Q_OBJECT

public:
  explicit Database(QObject* parent = nullptr);
  explicit Database(const QString& _folder_name);
  ~Database() = default;

  bool open_database(const QString& _folder_name);
  bool new_database(const QString& name, const QString& _description, const QString& _currency_iso,
                    const QLocale& _language);

  // no copy
  Database(const Database&)            = delete;
  Database& operator=(const Database&) = delete;
  Database(Database&&)                 = default;
  Database& operator=(Database&&)      = default;

  // getters
  [[nodiscard]] QString      get_file_name() const;
  [[nodiscard]] QSqlDatabase get_sql_db();
  [[nodiscard]] DB_Settings& get_settings()
  {
    return settings;
  }

  [[nodiscard]] bool is_valid() const
  {
    return database_valid;
  }


  // database file manager
  void save(const QString& _save_method = "");
  void remove();
  bool update_metadata(const QString& _save_method = "");

  // database queries
  int                                    last_insert_id();
  std::vector<std::tuple<int, int>>      find_rents_on_year(int year, int property_id);
  int                                    is_rent_exist(EMonth month, int year, int property_id, bool msg = false);
  std::tuple<float, float, float, float> total_year_rent_sum(int year, int property_id);
  std::optional<QSqlQuery>               find_property_rents_from_tenant_date(int tenant_id, const QDate& start_date,
                                                                              const QDate& end_date);
  bool                                   tenant_used(int tenant_id);
  bool                                   property_used(int property_id);
  bool                                   is_valid_column(ETable table, const QString& column);
  QString                                primaryKey(ETable table);

  std::optional<QSqlQuery> find_record(ETable table, int record_id);
  std::optional<QSqlQuery> all_records(ETable table);
  int                      get_record_id(ETable table, const QString& col_name, const QVariant& key);
  QString                  get_table_file_path(ETable table);

  // record operation
  // returns record id
  int  insert_record(ETable table, bool ask_msg = true, const QString& ask_info = "");
  bool update_record(ETable table, int record_id, const QString& column, const QVariant& value, bool ask_msg = false);
  bool delete_record(ETable table, int record_id, bool warning_msg = true, const QString& w_info = "");

  // file operation
  // returns file uuid
  QString insert_file(ETable table, int target_record_id, const QString& source_path, const QString& extension,
                      const QString& info_name);
  bool    update_file_info_name(ETable table, const QString& _uuid, const QString& info_name);
  bool    delete_file(ETable table, const QString& uuid, bool warning_msg = true, const QString& w_info = "");

  QString get_folder_name() const;
  QString get_database_path() const;
  QString get_file_path() const;
  QString get_settings_path() const;

  bool check_db_integrity() const;

private:
  DB_Settings settings;
  bool        database_valid = false;

public:
  static bool query_check(QSqlQuery* query, const QString& tag);


signals:
  void signal_db_updated();
};

#endif // DATABASE_H
