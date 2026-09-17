#include "record/record.h"

#include "database/manager.h"
#include "database/recorder.h"
#include "entity/attachment.h"
#include "entity/damage.h"
#include "entity/invoice.h"
#include "entity/landlord.h"
#include "entity/lease_agreement.h"
#include "entity/maintenance.h"
#include "entity/property.h"
#include "entity/property_feature.h"
#include "entity/property_room.h"
#include "entity/receipt.h"
#include "entity/rent.h"
#include "entity/tenant.h"


qsizetype create_record(ETable table)
{
  return Database_Manager::current_recorder()->insert_record(table, false);
}


bool contains_record(ETable table, qsizetype id)
{
  if (table == ETable::NONE || id == INVALID_ID) return false;

  return Database_Manager::current_recorder()->contains_record(table, id);
}


bool delete_record(ETable table, qsizetype id, bool wmsg, const QString& msg)
{
  if (table == ETable::NONE || id == INVALID_ID) return false;

  return Database_Manager::current_recorder()->delete_record(table, id, wmsg, msg);
}


template <RecordType T>
T from_sql(QSqlQuery& query)
{
  T out;

  if (!query.isValid() && !query.next()) return {};

  std::apply([&](const auto&... fields) { (fields.read(out, query), ...); }, T::sql_fields());
  out.id = query.value("id").toLongLong();

  return out;
}


template <RecordType T>
T read_record(qsizetype id)
{
  if (id == INVALID_ID) return {};

  auto valid_sql = Database_Manager::current_recorder()->find_record(T::static_table, id);

  if (!valid_sql) return {};

  return from_sql<T>(valid_sql.value());
}


template <RecordType T>
bool save_record(T* rec)
{
  if (!rec) return false;

  auto valid_sql = Database_Manager::current_recorder()->find_record(T::static_table, rec->id);

  if (!valid_sql) return false;

  return save_record(valid_sql.value(), *rec);
}

template <RecordType T>
QList<T> all_records()
{
  auto valid_sql = Database_Manager::current_recorder()->all_records(T::static_table);
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
  auto valid_sql = Database_Manager::current_recorder()->all_records_id(table);
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
  template T        from_sql<T>(QSqlQuery & query);                                                                    \
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