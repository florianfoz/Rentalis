#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include "record/field/field.h"
#include "record/record.h"

#include <QDate>
#include <QString>

class Maintenance : public Record<Maintenance>
{
public:
  static constexpr ETable static_table = ETable::Property_Room;


  qsizetype damage_id   = INVALID_ID;
  qsizetype property_id = INVALID_ID;
  QString   description;
  QDate     start_date;
  QDate     end_date;
  float     cost = 0.0F;
  EStatus   status;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"damage_id", &Maintenance::damage_id),     //
        field(u"property_id", &Maintenance::property_id), //
        field(u"description", &Maintenance::description), //
        field(u"start_date", &Maintenance::start_date),   //
        field(u"end_date", &Maintenance::end_date),       //
        field(u"cost", &Maintenance::cost),               //
        field(u"status", &Maintenance::status),           //
    };
  }

  bool operator==(const Maintenance& other) const noexcept
  {
    return damage_id == other.damage_id &&     //
           property_id == other.property_id && //
           description == other.description && //
           start_date == other.start_date &&   //
           end_date == other.end_date &&       //
           cost == other.cost &&               //
           status == other.status;
  }
};

#endif // MAINTENANCE_H
