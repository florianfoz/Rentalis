#include "landlord.h"

#include "base.h"

#include <QSqlQuery>

void Landlord::ensure_loaded()
{
  if (loaded || dirty) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::landlords, id);

  if (!query->next()) {
    qDebug() << "Landlord id (" << id << ") is invalid !";
    dirty  = true;
    loaded = false;
    return;
  }

  first_name  = query->value("first_name").toString();
  last_name   = query->value("last_name").toString();
  email       = query->value("email").toString();
  phone       = query->value("phone").toString();
  address     = query->value("address").toString();
  title       = int_to_ETitle(query->value("title_id").toInt());
  entity_type = int_to_EEntityType(query->value("entity_type_id").toInt());
  birthday    = query->value("birthday").toDate();
  singing     = Image_Buffer(query->value("singing").toByteArray(), query->value("singing_type").toString());
  icon        = Image_Buffer(query->value("icon").toByteArray(), query->value("icon_type").toString());
  dirty       = false;
  loaded      = true;
}

QString Landlord::get_full_name()
{
  ensure_loaded();
  return ETitle_to_str(title) + " " + first_name + " " + last_name;
}

bool Landlord::delete_record()
{
  if (!loaded && !dirty) {
    qDebug() << "Cannot delete: Landlord not loaded.";
    return false;
  }

  if (!DB_MANAGER.get_db()->delete_record(ETable::landlords, id, true, get_full_name())) {
    return false;
  }

  // reset
  first_name.clear();
  last_name.clear();
  title = ETitle::None;
  email.clear();
  phone.clear();
  address.clear();
  birthday    = QDate();
  entity_type = EEntityType::None;
  singing     = Image_Buffer();
  icon        = Image_Buffer();

  loaded = false;
  dirty  = false;
  return true;
}

int Landlord::insert_record()
{
  int result = DB_MANAGER.get_db()->insert_record(ETable::landlords, false);
  if (result < 0) return -1;

  id = result;
  update_record();
  return result;
}

bool Landlord::update_record()
{
  if (!DB_MANAGER.get_db()->find_record(ETable::landlords, id)) {
    qDebug() << "Update impossible, id (" << id << ") doesn't exist";
    return false;
  }


  bool issues[] = {
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "first_name", first_name),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "last_name", last_name),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "title_id", static_cast<int>(title)),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "email", email),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "phone", phone),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "address", address),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "birthday", birthday),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "singing", singing.get_blob()),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "singing_type", singing.get_image_type()),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "icon", icon.get_blob()),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "icon_type", icon.get_image_type()),
      DB_MANAGER.get_db()->update_record(ETable::landlords, id, "entity_type_id", static_cast<int>(entity_type)),
  };

  for (bool success : issues) {
    if (!success) {
      qDebug() << "Update failed for Landlord id" << id;
      return false;
    }
  }

  loaded = true;
  dirty  = false;
  return true;
}
