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

  int       property_id = -1;
  QString   description;
  QDateTime reported_date;
  ESeverity severity;
  float     cost = 0.0F;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"property_id", &Lease_Agreement::property_id),     //
        field(u"description", &Lease_Agreement::description),     //
        field(u"reported_date", &Lease_Agreement::reported_date), //
        field(u"severity", &Lease_Agreement::severity),           //
        field(u"cost", &Lease_Agreement::cost),                   //
    };
  }
};

#endif // LEASE_AGREEMENT_H
