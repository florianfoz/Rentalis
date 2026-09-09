#include "invoice.h"

#include "base.h"

#include <QDate>
#include <QSqlQuery>
#include <QString>


void Invoice::ensure_loaded()
{
  if (loaded || dirty) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::invoices, id);

  if (!query->next()) {
    qDebug() << "Invoice id (" << id << ") is invalid !";
    return;
  }

  tenant_id          = query->value("tenant_id").toInt();
  start_date         = query->value("start_date").toDate();
  end_date           = query->value("end_date").toDate();
  last_water_check   = query->value("last_water_check").toInt();
  water_check        = query->value("water_check").toInt();
  water_price        = query->value("water_price").toFloat();
  water_subscription = query->value("water_subscription").toFloat();
  charge_collected   = query->value("charge_collected").toFloat();
  other              = query->value("other").toFloat();
  comment            = query->value("comment").toString();

  loaded = true;
}

bool Invoice::delete_record()
{
  if (!loaded && !dirty) {
    qDebug() << "Cannot delete: Tenant not loaded.";
    return false;
  }

  if (!DB_MANAGER.get_db()->delete_record(ETable::tenants, id)) {
    return false;
  }

  // reset
  tenant_id          = -1;
  start_date         = QDate();
  end_date           = QDate();
  last_water_check   = 0.0f;
  water_check        = 0.0f;
  water_price        = 0.0f;
  water_subscription = 0.0f;
  charge_collected   = 0.0f;
  other              = 0.0f;
  comment.clear();

  loaded = false;
  dirty  = false;
  return true;
}

int Invoice::insert_record()
{
  int result = DB_MANAGER.get_db()->insert_record(ETable::invoices, false);
  if (result < 0) return -1;

  id = result;
  update_record();
  return result;
}

bool Invoice::update_record()
{
  if (!DB_MANAGER.get_db()->find_record(ETable::invoices, id)) {
    qDebug() << "Update invoices impossible, id (" << id << ") doesn't exists";
    return false;
  }

  bool issues[] = {
      DB_MANAGER.get_db()->update_record(ETable::invoices, id, "tenant_id", tenant_id),
      DB_MANAGER.get_db()->update_record(ETable::invoices, id, "start_date", start_date),
      DB_MANAGER.get_db()->update_record(ETable::invoices, id, "end_date", end_date),
      DB_MANAGER.get_db()->update_record(ETable::invoices, id, "last_water_check", last_water_check),
      DB_MANAGER.get_db()->update_record(ETable::invoices, id, "water_check", water_check),
      DB_MANAGER.get_db()->update_record(ETable::invoices, id, "water_price", water_price),
      DB_MANAGER.get_db()->update_record(ETable::invoices, id, "water_subscription", water_subscription),
      DB_MANAGER.get_db()->update_record(ETable::invoices, id, "charge_collected", charge_collected),
      DB_MANAGER.get_db()->update_record(ETable::invoices, id, "other", other),
      DB_MANAGER.get_db()->update_record(ETable::invoices, id, "comment", comment),
  };

  for (auto& success : issues) {
    if (!success) {
      qDebug() << "Update failed for Invoices id" << id;
      return false;
    }
  }

  loaded = true;
  dirty  = false;
  return true;
}
