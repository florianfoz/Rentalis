#ifndef RECEIPT_H
#define RECEIPT_H

#include "forward.h"
#include "record/field/field.h"
#include "record/record.h"

#include <QDate>
#include <QSqlQuery>
#include <QString>

class Receipt : public Record<Receipt>
{
public:
  static constexpr ETable static_table = ETable::Receipt;

  [[nodiscard]] float get_total_receivable() const;
  [[nodiscard]] float get_tenant_receivable() const;

  [[nodiscard]]
  std::optional<QSqlQuery> get_rent_references() const;


  int     tenant_id = -1;
  QDate   start_date;
  QDate   end_date;
  float   charge_advance = 0.0F;
  EDue    due_id;
  QDate   pay_date;
  QString comment;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"tenant_id", &Receipt::tenant_id),           //
        field(u"start_date", &Receipt::start_date),         //
        field(u"end_date", &Receipt::end_date),             //
        field(u"charge_advance", &Receipt::charge_advance), //
        field(u"due_id", &Receipt::due_id),                 //
        field(u"pay_date", &Receipt::pay_date),             //
        field(u"comment", &Receipt::comment),               //
    };
  }
};

#endif // RECEIPT_H
