#include "receipt.h"

#include "base.h"
#include "rent.h"

#include <QSqlQuery>

void Receipt::ensure_loaded() const
{
  if (loaded | dirty) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::receipts, id);

  if (!query->next()) {
    qDebug() << "Receipt id (" << id << ") is invalid !";
    return;
  }

  tenant_id      = query->value("tenant_id").toInt();
  start_date     = query->value("start_date").toDate();
  end_date       = query->value("end_date").toDate();
  charge_advance = query->value("charge_advance").toFloat();
  due_id         = int_to_EDue(query->value("due_id").toInt());
  pay_date       = query->value("pay_date").toDate();
  comment        = query->value("comment").toString();

  loaded = true;
}

float Receipt::get_total_receivable() const
{
  if (auto rent = get_rent_references(); rent.has_value() && rent->next()) {
    if (rent->next()) {
      return rent->value("rent").toFloat() + rent->value("housing_aid").toFloat() + rent->value("charge").toFloat()
             - charge_advance;
    }
  }
  return 0;
}

float Receipt::get_tenant_receivable() const
{
  if (auto q_rent = get_rent_references()) {
    float sum = 0.0f;
    while (q_rent->next()) {
      const float rent   = q_rent->value("rent").toFloat();
      const float aid    = q_rent->value("housing_aid").toFloat();
      const float charge = q_rent->value("charge").toFloat();
      sum += rent + aid - charge;
    }
    return sum;
  } else {
    return 0;
  }
}

std::optional<QSqlQuery> Receipt::get_rent_references() const
{
  return DB_MANAGER.get_db()->find_property_rents_from_tenant_date(get_tenant_id(), get_start_date(), get_end_date());
}

bool Receipt::delete_record()
{
  if (!loaded && !dirty) {
    qDebug() << "Cannot delete: Receipt not loaded.";
    return false;
  }

  if (!DB_MANAGER.get_db()->delete_record(ETable::receipts, id)) {
    return false;
  }

  // reset
  tenant_id      = -1;
  start_date     = QDate();
  end_date       = QDate();
  charge_advance = 0.0f;
  due_id         = EDue::None;
  pay_date       = QDate();
  comment.clear();

  loaded = false;
  dirty  = false;
  return true;
}

int Receipt::insert_record()
{
  int result = DB_MANAGER.get_db()->insert_record(ETable::receipts, false);
  if (result < 0) return -1;

  id = result;
  update_record();
  return result;
}

bool Receipt::update_record()
{
  if (!DB_MANAGER.get_db()->find_record(ETable::receipts, id)) {
    qDebug() << "Update receipts impossible, id (" << id << ") doesn't exists";
    return false;
  }

  bool issues[] = {
      DB_MANAGER.get_db()->update_record(ETable::receipts, id, "tenant_id", tenant_id),
      DB_MANAGER.get_db()->update_record(ETable::receipts, id, "start_date", start_date),
      DB_MANAGER.get_db()->update_record(ETable::receipts, id, "end_date", end_date),
      DB_MANAGER.get_db()->update_record(ETable::receipts, id, "charge_advance", charge_advance),
      DB_MANAGER.get_db()->update_record(ETable::receipts, id, "due_id", static_cast<int>(due_id)),
      DB_MANAGER.get_db()->update_record(ETable::receipts, id, "pay_date", pay_date),
      DB_MANAGER.get_db()->update_record(ETable::receipts, id, "comment", comment),
  };

  for (auto& success : issues) {
    if (!success) {
      qDebug() << "Update failed for Receipt id" << id;
      return false;
    }
  }

  loaded = true;
  dirty  = false;
  return true;
}
