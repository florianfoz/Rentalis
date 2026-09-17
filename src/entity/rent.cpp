#include "entity/rent.h"

#include "database/database.h"
#include "database/manager.h"

#include <QSqlQuery>


Rent Rent::load_from_tenant(qsizetype tenant_id, const QDate& start_date)
{
  QSqlQuery query(Database_Manager::current_sql());
  query.prepare(R"(
        SELECT *
        FROM rent
        WHERE rent.tenant_id = :tenant_id
        AND rent.date >= :start_date
        AND rent.date < :end_date
    )");

  query.bindValue(":tenant_id", tenant_id);
  query.bindValue(":start_date", start_date.toString(Qt::ISODate));
  query.bindValue(":end_date", start_date.addMonths(1).toString(Qt::ISODate));

  if (!Database_Manager::current_recorder()->query_check(&query,
                                                         "rent load from tenant_id=" + QString::number(tenant_id))
      || !query.next())
    return {};

  return from_sql<Rent>(query);
}

Rent Rent::load_from_property(qsizetype _property_id, const QDate& start_date)
{
  QSqlQuery query(Database_Manager::current_sql());
  query.prepare(R"(
        SELECT *
        FROM rent
        WHERE rent.property_id = :property_id
        AND rent.date >= :start_date
        AND rent.date < :end_date
    )");
  query.bindValue(":property_id", _property_id);
  query.bindValue(":start_date", start_date.toString(Qt::ISODate));
  query.bindValue(":end_date", start_date.addMonths(1).toString(Qt::ISODate));

  if (!Database_Manager::current_recorder()->query_check(&query, "rent load from rent.property_id="
                                                                     + QString::number(_property_id))
      || !query.next())
    return {};

  return from_sql<Rent>(query);
}


// SELECT month, rent_id
QList<std::tuple<int, qsizetype>> Rent::find_by_year_property(int year, qsizetype property_id)
{
  QList<std::tuple<int, qsizetype>> vec;
  vec.reserve(12);

  QSqlQuery query(Database_Manager::current_sql());
  query.prepare(R"(
            SELECT
                strftime("%m", rent.date) as month,
                rent.id as rent_id
            FROM rent
            INNER JOIN property
                ON property.id = rent.property_id
            WHERE property.id = :property_id
            AND rent.date >= :start_date
            AND rent.date < :end_date
            ORDER BY month
        )");

  QDate start_date(year, 1, 1);
  QDate end_date = start_date.addYears(1);

  query.bindValue(":start_date", start_date.toString(Qt::ISODate));
  query.bindValue(":end_date", end_date.toString(Qt::ISODate));

  // name of the property only
  query.bindValue(":property_id", property_id);

  if (!query.exec()) {
    qDebug() << "Error SELECT rent:" << query.lastError().text();
    return vec;
  }

  while (query.next()) {
    vec.push_back({query.value("month").toInt(), query.value("id").toInt()});
  }

  return vec;
}

qsizetype Rent::is_exists(EMonth month, int year, qsizetype property_id, bool msg)
{
  QSqlQuery query(Database_Manager::current_sql());
  query.prepare(R"(
            SELECT id
            FROM rent
            WHERE rent.property_id = :property_id
            AND rent.date >= :start_date
            AND rent.date < :end_date;
        )");
  QDate start(year, static_cast<int>(month), 1);
  QDate end = start.addMonths(1);

  query.bindValue(":start_date", start.toString(Qt::ISODate));
  query.bindValue(":end_date", end.toString(Qt::ISODate));

  query.bindValue(":property_id", property_id);

  if (query.exec() && query.next()) {
    return query.value("id").toInt();
  }
  return INVALID_ID;
}

// SUM of rent, housing_aid, charge, household_waste
std::tuple<float, float, float, float> Rent::year_sum(int year, qsizetype property_id)
{
  QSqlQuery query(Database_Manager::current_sql());
  query.prepare(R"(
            SELECT
                SUM(rent) as rent_val,
                SUM(housing_aid) as housing_aid,
                SUM(charge) as charge,
                SUM(household_waste) as household_waste
            FROM rent
            WHERE rent.property_id = :property_id
            AND rent.date >= :start_date
            AND rent.date < :end_date;
        )");

  QDate start_date(year, 1, 1);
  QDate end_date = start_date.addYears(1);

  query.bindValue(":property_id", property_id);
  query.bindValue(":start_date", start_date.toString(Qt::ISODate));
  query.bindValue(":end_date", end_date.toString(Qt::ISODate));

  if (Database_Manager::current_recorder()->query_check(&query, "total year rent sum") && query.next()) {
    return {query.value("rent_val").toFloat(), query.value("housing_aid").toFloat(), query.value("charge").toFloat(),
            query.value("household_waste").toFloat()};
  }

  return {};
}

// find the linked rent and property  according to the receipt.tenant_id and receipt.start_date
// SELECT rent_id, rent_val, name, address
std::optional<QSqlQuery> Rent::find_by_tenant_date(qsizetype tenant_id, const QDate& start_date, const QDate& end_date)
{
  QSqlQuery query(Database_Manager::current_sql());
  query.prepare(R"(
            SELECT rent.id as rent_id, rent.rent as rent_val, property.name as name, property.address as address
            FROM property
            INNER JOIN rent
                ON rent.property_id = property.id
            WHERE rent.tenant_id = :tenant_id
            AND rent.date >= :start_date
            AND rent.date < :end_date
            ORDER BY rent.date DESC;
        )");

  QDate _start_date(start_date.year(), start_date.month(), 1);
  QDate _end_date(start_date.year(), start_date.month(), 1);

  query.bindValue(":tenant_id", tenant_id);
  query.bindValue(":start_date", _start_date.toString(Qt::ISODate));
  query.bindValue(":end_date", _end_date.toString(Qt::ISODate));

  if (!query.exec()) return std::nullopt;

  return query;
}
