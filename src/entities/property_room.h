#ifndef PROPERTY_ROOM_H
#define PROPERTY_ROOM_H

#include "record/field/field.h"
#include "record/record.h"

#include <QString>


class Property_Room : public Record<Property_Room>
{
public:
  static constexpr ETable static_table = ETable::Property_Room;

  int       property_id  = -1;
  ERoomType room_type    = static_cast<ERoomType>(0);
  float     surface_size = 0.0F;
  bool      is_furnished = false;
  QString   comment;
  int       floor;
  QString   icon;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"property_id", &Property_Room::property_id),   //
        field(u"room_type", &Property_Room::room_type),       //
        field(u"surface_size", &Property_Room::surface_size), //
        field(u"is_furnished", &Property_Room::is_furnished), //
        field(u"comment", &Property_Room::comment),           //
        field(u"floor", &Property_Room::floor),               //
        field(u"icon", &Property_Room::icon),                 //
    };
  }
};

#endif // PROPERTY_ROOM_H
