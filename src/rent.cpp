#include "rent.h"

#include "base.h"

#include <QSqlQuery>

Rent Rent::load_from_tenant(int tenant_id, const QDate& start_date)
{
  Rent      rent(-1, true);
  QSqlQuery query(DB_MANAGER.get_db()->get_sql_db());
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

  if (DB_MANAGER.get_db()->query_check(&query, "rent load from tenant_id=" + QString::number(tenant_id))
      && query.next()) {
    int id = query.value("rent_id").toInt();
    rent   = Rent(id);
  }

  return rent;
}

Rent Rent::load_from_property(int _property_id, const QDate& start_date)
{
  Rent      rent(-1, true);
  QSqlQuery query(DB_MANAGER.get_db()->get_sql_db());
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

  if (DB_MANAGER.get_db()->query_check(&query, "rent load from property_id=" + QString::number(_property_id))
      && query.next()) {
    int id = query.value("rent_id").toInt();
    rent   = Rent(id);
  }

  return rent;
}

bool Rent::delete_record()
{
  if (!loaded && !dirty) {
    qDebug() << "Cannot delete: Tenant not loaded.";
    return false;
  }

  if (!DB_MANAGER.get_db()->delete_record(ETable::rents, id)) {
    return false;
  }

  // reset
  id              = -1;
  date            = QDate();
  rent            = 0.0f;
  housing_aid     = 0.0f;
  charge          = 0.0f;
  household_waste = 0.0f;
  property_id     = -1;
  tenant_id       = -1;
  comment.clear();

  loaded = false;
  dirty  = false;
  return true;
}

int Rent::insert_record()
{
  int result = DB_MANAGER.get_db()->insert_record(ETable::rents, false);
  if (result < 0) return -1;

  id = result;
  update_record();
  return result;
}

bool Rent::update_record()
{
  if (!DB_MANAGER.get_db()->find_record(ETable::rents, id)) {
    qDebug() << "Update rents impossible, id (" << id << ") doesn't exists";
    return false;
  }

  bool issues[] = {
      DB_MANAGER.get_db()->update_record(ETable::rents, id, "date", date),
      DB_MANAGER.get_db()->update_record(ETable::rents, id, "rent", rent),
      DB_MANAGER.get_db()->update_record(ETable::rents, id, "housing_aid", housing_aid),
      DB_MANAGER.get_db()->update_record(ETable::rents, id, "charge", charge),
      DB_MANAGER.get_db()->update_record(ETable::rents, id, "household_waste", household_waste),
      DB_MANAGER.get_db()->update_record(ETable::rents, id, "property_id", property_id),
      DB_MANAGER.get_db()->update_record(ETable::rents, id, "tenant_id", tenant_id),
      DB_MANAGER.get_db()->update_record(ETable::rents, id, "comment", comment),
  };

  for (auto& success : issues) {
    if (!success) {
      qDebug() << "Update failed for Rents id" << id;
      return false;
    }
  }

  loaded = true;
  dirty  = false;
  return true;
}


void Rent::ensure_loaded()
{
  if (loaded || dirty) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::rents, id);

  if (!query->next()) {
    qDebug() << "Rent id (" << id << ") is invalid !";
    return;
  }

  id              = query->value("rent_id").toInt();
  date            = query->value("date").toDate();
  rent            = query->value("rent").toFloat();
  housing_aid     = query->value("housing_aid").toFloat();
  charge          = query->value("charge").toFloat();
  household_waste = query->value("household_waste").toFloat();
  property_id     = query->value("property_id").toInt();
  tenant_id       = query->value("tenant_id").toInt();
  comment         = query->value("comment").toString();

  loaded = true;
}
