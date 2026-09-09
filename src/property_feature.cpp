#include "property_feature.h"

#include "base.h"
#include "image_buffer.h"

#include <QSqlQuery>


void Property_Feature::ensure_loaded()
{
  if (loaded || dirty) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::property_feature, id);

  if (!query->next()) {
    qDebug() << "property_feature id (" << id << ") is invalid !";
    dirty  = true;
    loaded = false;
    return;
  }

  property_id  = query->value("property_id").toInt();
  feature_type = int_to_EFeatureType(query->value("feature_type_id").toInt());
  surface_size = query->value("surface_size").toFloat();
  comment      = query->value("comment").toString();
  is_indoor    = query->value("is_indoor").toBool();
  icon = std::make_unique<Image_Buffer>(query->value("icon").toByteArray(), query->value("icon_type").toString());

  dirty  = false;
  loaded = true;
}

bool Property_Feature::delete_record()
{
  if (!loaded && !dirty) {
    qDebug() << "Cannot delete: property_feature not loaded.";
    return false;
  }

  if (!DB_MANAGER.get_db()->delete_record(ETable::property_feature, id)) {
    return false;
  }

  // reset
  property_id  = -1;
  feature_type = EFeatureType::None;
  surface_size = 0.0f;
  comment.clear();
  is_indoor = false;
  icon.release();

  loaded = false;
  dirty  = false;
  return true;
}

int Property_Feature::insert_record()
{
  int result = DB_MANAGER.get_db()->insert_record(ETable::property_feature, false);
  if (result < 0) return -1;

  id = result;
  update_record();
  return result;
}

bool Property_Feature::update_record()
{
  if (!DB_MANAGER.get_db()->find_record(ETable::property_feature, id)) {
    qDebug() << "Update impossible, id (" << id << ") doesn't exist";
    return false;
  }

  bool issues[] = {
      DB_MANAGER.get_db()->update_record(ETable::property_feature, id, "property_id", property_id),
      DB_MANAGER.get_db()->update_record(ETable::property_feature, id, "feature_type_id",
                                         static_cast<int>(feature_type)),
      DB_MANAGER.get_db()->update_record(ETable::property_feature, id, "surface_size", surface_size),
      DB_MANAGER.get_db()->update_record(ETable::property_feature, id, "comment", comment),
      DB_MANAGER.get_db()->update_record(ETable::property_feature, id, "is_indoor", is_indoor),
      DB_MANAGER.get_db()->update_record(ETable::property_feature, id, "icon", icon->get_blob()),
      DB_MANAGER.get_db()->update_record(ETable::property_feature, id, "icon_type", icon->get_image_type()),
  };

  for (bool success : issues) {
    if (!success) {
      qDebug() << "Update failed for property_feature id" << id;
      return false;
    }
  }

  loaded = true;
  dirty  = false;
  return true;
}
