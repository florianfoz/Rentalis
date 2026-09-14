#include "record/record.h"

#include "database/database.h"
#include "database/manager.h"
#include "entities/attachment.h"
#include "entities/damage.h"
#include "entities/invoice.h"
#include "entities/landlord.h"
#include "entities/lease_agreement.h"
#include "entities/maintenance.h"
#include "entities/property.h"
#include "entities/property_feature.h"
#include "entities/property_room.h"
#include "entities/receipt.h"
#include "entities/rent.h"
#include "entities/tenant.h"


qsizetype create_record(ETable table)
{
  return Database_Manager::current_database()->insert_record(table, false);
}


bool contains_record(ETable table, qsizetype id)
{
  if (table == ETable::NONE || id == INVALID_ID) return false;

  return Database_Manager::current_database()->contains_record(table, id);
}


bool delete_record(ETable table, qsizetype id, bool wmsg, const QString& msg)
{
  if (table == ETable::NONE || id == INVALID_ID) return false;

  return Database_Manager::current_database()->delete_record(table, id, wmsg, msg);
}


template <RecordType T>
T from_sql(const QSqlQuery& query)
{
  T out;

  std::apply([&](const auto&... fields) { (fields.read(out, query), ...); }, T::sql_fields());

  out.id = query.value("id").toLongLong();

  return out;
}


template <RecordType T>
T read_record(qsizetype id)
{
  auto valid_sql = Database_Manager::current_database()->find_record(T::static_table, id);

  if (!valid_sql) return {};

  return from_sql<T>(valid_sql.value());
}


template <RecordType T>
bool save_record(T* rec)
{
  if (!rec) return false;

  auto valid_sql = Database_Manager::current_database()->find_record(T::static_table, rec->id);

  if (!valid_sql) return false;

  return save_record(valid_sql.value(), *rec);
}

template <RecordType T>
QList<T> all_records()
{
  auto valid_sql = Database_Manager::current_database()->all_records(T::static_table);
  if (!valid_sql) return {};

  auto& sql = valid_sql.value();

  if (!sql.isActive()) return {};

  QList<T> out;
  while (sql.next()) {
    out.append(from_sql<T>(sql));
  }

  return out;
}

QList<qsizetype> all_records_id(ETable table)
{
  auto valid_sql = Database_Manager::current_database()->all_records_id(table);
  if (!valid_sql) return {};

  auto& sql = valid_sql.value();

  if (!sql.isActive()) return {};

  QList<qsizetype> out;
  while (sql.next()) {
    out.append(sql.value("id").toInt());
  }

  return out;
}


// Explicit template instantiations

#define INSTANTIATE_RECORD(T)                                                                                          \
  template T        read_record<T>(qsizetype id);                                                                      \
  template T        from_sql<T>(const QSqlQuery& query);                                                               \
  template bool     save_record<T>(T * rec);                                                                           \
  template QList<T> all_records<T>();

INSTANTIATE_RECORD(Property)
INSTANTIATE_RECORD(Invoice)
INSTANTIATE_RECORD(Receipt)
INSTANTIATE_RECORD(Rent)
INSTANTIATE_RECORD(Tenant)
INSTANTIATE_RECORD(Landlord)
INSTANTIATE_RECORD(Damage)
INSTANTIATE_RECORD(Maintenance)
INSTANTIATE_RECORD(Lease_Agreement)
INSTANTIATE_RECORD(Property_Room)
INSTANTIATE_RECORD(Property_Feature)
INSTANTIATE_RECORD(Attachment)

#undef INSTANTIATE_RECORD