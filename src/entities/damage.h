#ifndef DAMAGE_H
#define DAMAGE_H

#include "record/field/field.h"
#include "record/record.h"

#include <QDate>
#include <QString>


class Damage : public Record<Damage>
{
public:
  static constexpr ETable static_table = ETable::Damage;

  int       property_id = -1;
  QString   description;
  QDate     reported_date;
  ESeverity severity;
  float     cost = 0.0F;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"property_id", &Damage::property_id),     //
        field(u"description", &Damage::description),     //
        field(u"reported_date", &Damage::reported_date), //
        field(u"severity", &Damage::severity),           //
        field(u"cost", &Damage::cost),                   //
    };
  };
};

#endif // DAMAGE_H
