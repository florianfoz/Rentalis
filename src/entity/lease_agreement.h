#ifndef LEASE_AGREEMENT_H
#define LEASE_AGREEMENT_H

#include "record/field/field.h"
#include "record/record.h"

#include <QDate>
#include <QString>


class Lease_Agreement : public Record<Lease_Agreement>
{
public:
  static constexpr ETable static_table = ETable::Lease_Agreement;

  qsizetype  property_id = INVALID_ID;
  qsizetype  tenant_id   = INVALID_ID;
  QString    comment;
  QDateTime  start_date;
  QDateTime  end_date;
  ELeaseType lease_type;
  float      cost = 0.0F;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"property_id", &Lease_Agreement::property_id), //
        field(u"comment", &Lease_Agreement::comment),         //
        field(u"start_date", &Lease_Agreement::start_date),   //
        field(u"end_date", &Lease_Agreement::end_date),       //
        field(u"lease_type", &Lease_Agreement::lease_type),   //
        field(u"cost", &Lease_Agreement::cost),               //
        field(u"tenant_id", &Lease_Agreement::tenant_id),     //
    };
  }

  bool operator==(const Lease_Agreement& other) const noexcept
  {
    return property_id == other.property_id && //
           tenant_id == other.tenant_id &&     //
           comment == other.comment &&         //
           start_date == other.start_date &&   //
           end_date == other.end_date &&       //
           lease_type == other.lease_type &&   //
           cost == other.cost;
  }
};

#endif // LEASE_AGREEMENT_H
