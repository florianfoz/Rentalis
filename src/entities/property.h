#ifndef PROPERTY_H
#define PROPERTY_H

#include "record/field/field.h"
#include "record/record.h"

#include <QString>

class Property : public Record<Property>
{
public:
  static constexpr ETable static_table = ETable::Property;

  QString       name;
  QString       address;
  QString       country;
  QString       location_number;
  QString       icon;
  QString       city;
  QString       postal_code;
  EPropertyType property_type;
  int           surface = 9;
  ECondition    condition;
  EEnergy       energy;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"name", &Property::name),
        field(u"address", &Property::address),
        field(u"country", &Property::country),
        field(u"location_number", &Property::location_number),
        field(u"city", &Property::city),
        field(u"postal_code", &Property::postal_code),
        field(u"surface", &Property::surface),
        field(u"condition_id", &Property::condition),
        field(u"icon", &Property::icon),
        field(u"energy_class_id", &Property::energy),
        field(u"property_type_id", &Property::property_type),
    };
  };
};

#endif // PROPERTY_H
