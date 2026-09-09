#include "property.h"

#include "base.h"
#include "image_buffer.h"

#include <QSqlQuery>

void Property::ensure_loaded()
{
  if (loaded || dirty) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::properties, id);

  if (!query->next()) {
    qDebug() << "Property id (" << id << ") is invalid !";
    dirty  = true;
    loaded = false;
    return;
  }

  name            = query->value("name").toString();
  country         = query->value("country").toString();
  address         = query->value("address").toString();
  location_number = query->value("location_number").toString();
  city            = query->value("city").toString();
  postal_code     = query->value("postal_code").toString();
  surface         = query->value("surface").toInt();
  condition       = int_to_ECondition(query->value("condition_id").toInt());
  energy          = int_to_EEnergy(query->value("energy_class_id").toInt());
  property_type   = int_to_EPropertyType(query->value("property_type_id").toInt());
  icon            = Image_Buffer(query->value("icon").toByteArray(), query->value("icon_type").toString());
  files           = Record_Files(ETable::properties_files, id);

  dirty  = false;
  loaded = true;
}

bool Property::delete_record()
{
  if (!loaded && !dirty) {
    qDebug() << "Cannot delete: Property not loaded.";
    return false;
  }

  if (!DB_MANAGER.get_db()->delete_record(
          ETable::properties, id, true,
          QObject::tr("%1 at %2, %3, %4 [%3]").arg(name, address, postal_code, city, country))) {
    return false;
  }

  // reset
  name.clear();
  country.clear();
  address.clear();
  location_number.clear();
  city.clear();
  postal_code.clear();
  surface       = -1;
  condition     = ECondition::None;
  energy        = EEnergy::None;
  property_type = EPropertyType::None;
  icon.clear();
  files.delete_all_files();

  loaded = false;
  dirty  = false;
  return true;
}

int Property::insert_record()
{
  int result = DB_MANAGER.get_db()->insert_record(ETable::properties, false);
  if (result < 0) return -1;

  id = result;
  update_record();
  return result;
}

bool Property::update_record()
{
  if (!DB_MANAGER.get_db()->find_record(ETable::properties, id)) {
    qDebug() << "Update impossible, id (" << id << ") doesn't exist";
    return false;
  }

  bool issues[] = {
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "name", name),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "country", country),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "address", address),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "location_number", location_number),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "city", city),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "postal_code", postal_code),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "surface", surface),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "condition_id", static_cast<int>(condition)),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "icon", icon.get_blob()),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "icon_type", icon.get_image_type()),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "energy_class_id", static_cast<int>(energy)),
      DB_MANAGER.get_db()->update_record(ETable::properties, id, "property_type_id", static_cast<int>(property_type)),
  };

  for (bool success : issues) {
    if (!success) {
      qDebug() << "Update failed for properties id" << id;
      return false;
    }
  }

  loaded = true;
  dirty  = false;
  return true;
}
