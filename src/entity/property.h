#ifndef PROPERTY_H
#define PROPERTY_H

#include "record/field/field.h"
#include "record/record.h"

#include <QString>

class Property : public Record<Property>
{
public:
  static constexpr ETable static_table = ETable::Property;

  [[nodiscard]] static bool is_used(qsizetype property_id);


  QString       name;
  QString       address;
  QString       country;
  QString       location_number;
  QString       city;
  QString       postal_code;
  EPropertyType property_type;
  int           surface = 0;
  ECondition    condition;
  EEnergy       energy;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"name", &Property::name),                       //
        field(u"address", &Property::address),                 //
        field(u"country", &Property::country),                 //
        field(u"location_number", &Property::location_number), //
        field(u"city", &Property::city),                       //
        field(u"postal_code", &Property::postal_code),         //
        field(u"surface", &Property::surface),                 //
        field(u"condition", &Property::condition),             //
        field(u"energy_class", &Property::energy),             //
        field(u"property_type", &Property::property_type),     //
    };
  };

  bool operator==(const Property& other) const noexcept
  {
    return name == other.name &&                       //
           address == other.address &&                 //
           country == other.country &&                 //
           location_number == other.location_number && //
           city == other.city &&                       //
           postal_code == other.postal_code &&         //
           property_type == other.property_type &&     //
           surface == other.surface &&                 //
           condition == other.condition &&             //
           energy == other.energy;
  }
};

#endif // PROPERTY_H
