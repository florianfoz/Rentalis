#ifndef INVOICE_H
#define INVOICE_H

#include "record/field/field.h"
#include "record/record.h"

#include <QDate>
#include <QString>


class Invoice : public Record<Invoice>
{
public:
  static constexpr ETable static_table = ETable::Invoice;

  [[nodiscard]] int get_water_consum() const
  {
    return water_check - last_water_check;
  }
  [[nodiscard]] float get_water_consum_price() const
  {
    return float(get_water_consum()) * water_price;
  }
  // rest for tenant to pay
  [[nodiscard]] float get_rest() const
  {
    return get_water_consum_price() + water_subscription - charge_collected;
  }
  [[nodiscard]] bool is_rest_for_tenant() const
  {
    return get_rest() >= 0;
  }

  qsizetype tenant_id = INVALID_ID;
  QDate     start_date;
  QDate     end_date;
  int       last_water_check   = 0;
  int       water_check        = 0;
  float     water_price        = 0.0F;
  float     water_subscription = 0.0F;
  float     charge_collected   = 0.0F;
  float     other              = 0.0F;
  QString   comment;


  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"tenant_id", &Invoice::tenant_id),
        field(u"start_date", &Invoice::start_date),
        field(u"end_date", &Invoice::end_date),
        field(u"last_water_check", &Invoice::last_water_check),
        field(u"water_check", &Invoice::water_check),
        field(u"water_price", &Invoice::water_price),
        field(u"water_subscription", &Invoice::water_subscription),
        field(u"charge_collected", &Invoice::charge_collected),
        field(u"other", &Invoice::other),
        field(u"comment", &Invoice::comment),
    };
  }

  bool operator==(const Invoice& _other) const noexcept
  {
    return tenant_id == _other.tenant_id &&                   //
           start_date == _other.start_date &&                 //
           end_date == _other.end_date &&                     //
           last_water_check == _other.last_water_check &&     //
           water_check == _other.water_check &&               //
           water_price == _other.water_price &&               //
           water_subscription == _other.water_subscription && //
           charge_collected == _other.charge_collected &&     //
           other == _other.other &&                           //
           comment == _other.comment;
  }
};

#endif // INVOICE_H
