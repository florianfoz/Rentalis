#include "tenant.h"

#include "base.h"

#include <QSqlQuery>

void Tenant::ensure_loaded()
{
  if (loaded || dirty) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::tenants, id);

  if (!query->next()) {
    qDebug() << "Landlord id (" << id << ") is invalid !";
    return;
  }

  first_name  = query->value("first_name").toString();
  last_name   = query->value("last_name").toString();
  title       = int_to_ETitle(query->value("title_id").toInt());
  email       = query->value("email").toString();
  phone       = query->value("phone").toString();
  birthday    = query->value("birthday").toDate();
  entity_type = int_to_EEntityType(query->value("entity_type_id").toInt());

  loaded = true;
}

QString Tenant::get_full_name()
{
  ensure_loaded();
  return ETitle_to_str(title) + " " + first_name + " " + last_name;
}

bool Tenant::delete_record()
{
  if (!loaded && !dirty) {
    qDebug() << "Cannot delete: Tenant not loaded.";
    return false;
  }

  if (!DB_MANAGER.get_db()->delete_record(ETable::tenants, id)) {
    return false;
  }

  // reset
  first_name.clear();
  last_name.clear();
  title = ETitle::None;
  email.clear();
  phone.clear();
  birthday    = QDate();
  entity_type = EEntityType::None;

  loaded = false;
  dirty  = false;
  return true;
}

int Tenant::insert_record()
{
  int result = DB_MANAGER.get_db()->insert_record(ETable::tenants, false);
  if (result < 0) return -1;

  id = result;
  update_record();
  return result;
}

bool Tenant::update_record()
{
  if (!DB_MANAGER.get_db()->find_record(ETable::tenants, id)) {
    qDebug() << "Update tenants impossible, id (" << id << ") doesn't exists";
    return false;
  }

  bool issues[] = {
      DB_MANAGER.get_db()->update_record(ETable::tenants, id, "first_name", first_name),
      DB_MANAGER.get_db()->update_record(ETable::tenants, id, "last_name", last_name),
      DB_MANAGER.get_db()->update_record(ETable::tenants, id, "title_id", static_cast<int>(title)),
      DB_MANAGER.get_db()->update_record(ETable::tenants, id, "email", email),
      DB_MANAGER.get_db()->update_record(ETable::tenants, id, "phone", phone),
      DB_MANAGER.get_db()->update_record(ETable::tenants, id, "birthday", birthday),
      DB_MANAGER.get_db()->update_record(ETable::tenants, id, "entity_type_id", static_cast<int>(entity_type)),
  };

  for (auto& success : issues) {
    if (!success) {
      qDebug() << "Update failed for Tenant id" << id;
      return false;
    }
  }

  loaded = true;
  dirty  = false;
  return true;
}
