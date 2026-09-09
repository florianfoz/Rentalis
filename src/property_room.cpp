#include "property_room.h"

#include "base.h"

#include <QBuffer>
#include <QSqlQuery>


void Property_Room::ensure_loaded()
{
  if (loaded || dirty) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::property_room, id);

  if (!query->next()) {
    qDebug() << "property_room id (" << id << ") is invalid !";
    dirty  = true;
    loaded = false;
    return;
  }

  property_id  = query->value("property_id").toInt();
  room_type    = int_to_ERoomType(query->value("room_type").toInt());
  surface_size = query->value("surface_size").toFloat();
  is_furnished = query->value("is_furnished").toBool();
  comment      = query->value("comment").toString();
  floor        = query->value("floor").toInt();
  icon         = Image_Buffer(query->value("icon").toByteArray(), query->value("icon_type").toString());

  dirty  = false;
  loaded = true;
}

bool Property_Room::delete_record()
{
  if (!loaded && !dirty) {
    qDebug() << "Cannot delete: property room not loaded.";
    return false;
  }

  if (!DB_MANAGER.get_db()->delete_record(ETable::property_room, id)) {
    return false;
  }

  // reset
  property_id  = -1;
  room_type    = ERoomType::None;
  surface_size = 0.0f;
  is_furnished = false;
  comment.clear();
  floor = 0;
  icon.clear();

  loaded = false;
  dirty  = false;
  return true;
}

int Property_Room::insert_record()
{
  int result = DB_MANAGER.get_db()->insert_record(ETable::property_room, false);
  if (result < 0) return -1;

  id = result;
  update_record();
  return result;
}

bool Property_Room::update_record()
{
  if (!DB_MANAGER.get_db()->find_record(ETable::property_room, id)) {
    qDebug() << "Update impossible, id (" << id << ") doesn't exist";
    return false;
  }

  bool issues[] = {
      DB_MANAGER.get_db()->update_record(ETable::property_room, id, "property_id", property_id),
      DB_MANAGER.get_db()->update_record(ETable::property_room, id, "room_type", static_cast<int>(room_type)),
      DB_MANAGER.get_db()->update_record(ETable::property_room, id, "surface_size", surface_size),
      DB_MANAGER.get_db()->update_record(ETable::property_room, id, "is_furnished", is_furnished),
      DB_MANAGER.get_db()->update_record(ETable::property_room, id, "comment", comment),
      DB_MANAGER.get_db()->update_record(ETable::property_room, id, "floor", floor),
      DB_MANAGER.get_db()->update_record(ETable::property_room, id, "icon", icon.get_blob()),
      DB_MANAGER.get_db()->update_record(ETable::property_room, id, "icon_type", icon.get_image_type()),
  };

  for (bool success : issues) {
    if (!success) {
      qDebug() << "Update failed for property_room id" << id;
      return false;
    }
  }

  loaded = true;
  dirty  = false;
  return true;
}
