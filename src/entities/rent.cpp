#include "entities/rent.h"

#include "database/database.h"
#include "database/manager.h"

#include <QSqlQuery>


Rent Rent::load_from_tenant(int tenant_id, const QDate& start_date)
{
  QSqlQuery query(Database_Manager::current_database()->sql());
  query.prepare(R"(
        SELECT *
        FROM rents
        WHERE rents.tenant_id = :tenant_id
        AND rents.date >= :start_date
        AND rents.date < :end_date
    )");

  query.bindValue(":tenant_id", tenant_id);
  query.bindValue(":start_date", start_date.toString(Qt::ISODate));
  query.bindValue(":end_date", start_date.addMonths(1).toString(Qt::ISODate));

  if (!Database::query_check(&query, "rent load from tenant_id=" + QString::number(tenant_id)) || !query.next())
    return {};

  return from_sql<Rent>(query);
}

Rent Rent::load_from_property(int _property_id, const QDate& start_date)
{
  QSqlQuery query(Database_Manager::current_database()->sql());
  query.prepare(R"(
        SELECT *
        FROM rents
        WHERE rents.property_id = :property_id
        AND rents.date >= :start_date
        AND rents.date < :end_date
    )");
  query.bindValue(":property_id", _property_id);
  query.bindValue(":start_date", start_date.toString(Qt::ISODate));
  query.bindValue(":end_date", start_date.addMonths(1).toString(Qt::ISODate));

  if (!Database::query_check(&query, "rent load from property_id=" + QString::number(_property_id)) || !query.next())
    return {};

  return from_sql<Rent>(query);
}
