#ifndef PROPERTY_FEATURE_H
#define PROPERTY_FEATURE_H

#include "record/field/field.h"
#include "record/record.h"

#include <QString>

class Property_Feature : public Record<Property_Feature>
{
public:
  static constexpr ETable static_table = ETable::Property_Feature;

  qsizetype    property_id  = -1;
  EFeatureType feature_type = static_cast<EFeatureType>(0);
  float        surface_size = 0.0F;
  bool         is_indoor    = false;
  QString      comment;
  QString      icon;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"property_id", &Property_Feature::property_id),   //
        field(u"feature_type", &Property_Feature::feature_type), //
        field(u"surface_size", &Property_Feature::surface_size), //
        field(u"is_indoor", &Property_Feature::is_indoor),       //
        field(u"comment", &Property_Feature::comment),           //
        field(u"icon", &Property_Feature::icon),                 //
    };
  }
};

#endif // PROPERTY_FEATURE_H
