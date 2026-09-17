#ifndef PROPERTY_ROOM_H
#define PROPERTY_ROOM_H

#include "record/field/field.h"
#include "record/record.h"

#include <QString>


class Property_Room : public Record<Property_Room>
{
public:
  static constexpr ETable static_table = ETable::Property_Room;

  qsizetype property_id = INVALID_ID;
  ERoomType room_type;
  float     surface_size = 0.0F;
  bool      is_furnished = false;
  QString   comment;
  int       floor;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"property_id", &Property_Room::property_id),   //
        field(u"room_type", &Property_Room::room_type),       //
        field(u"surface_size", &Property_Room::surface_size), //
        field(u"is_furnished", &Property_Room::is_furnished), //
        field(u"comment", &Property_Room::comment),           //
        field(u"floor", &Property_Room::floor),               //
    };
  }

  bool operator==(const Property_Room& other) const noexcept
  {
    return property_id == other.property_id &&   //
           room_type == other.room_type &&       //
           surface_size == other.surface_size && //
           is_furnished == other.is_furnished && //
           comment == other.comment &&           //
           floor == other.floor;
  }
};

#endif // PROPERTY_ROOM_H
