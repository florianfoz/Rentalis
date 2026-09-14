#include "entities/receipt.h"

#include "database/database.h"
#include "database/manager.h"

#include <QSqlQuery>

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
    float sum = 0.0F;
    while (q_rent->next()) {
      const float rent   = q_rent->value("rent").toFloat();
      const float aid    = q_rent->value("housing_aid").toFloat();
      const float charge = q_rent->value("charge").toFloat();
      sum += rent + aid - charge;
    }
    return sum;
  }

  return 0.0F;
}

std::optional<QSqlQuery> Receipt::get_rent_references() const
{
  return Database_Manager::current_database()->find_property_rents_from_tenant_date(tenant_id, start_date, end_date);
}
