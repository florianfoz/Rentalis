#include "maintenance.h"

#include "base.h"

#include <QSqlQuery>


void Maintenance::ensure_loaded()
{
  if (loaded || dirty) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::maintenances, id);

  if (!query->next()) {
    qDebug() << "maintenances id (" << id << ") is invalid !";
    dirty  = true;
    loaded = false;
    return;
  }

  damage_id   = query->value("damage_id").toInt();
  property_id = query->value("property_id").toInt();
  description = query->value("description").toString();
  start_date  = query->value("start_date").toDate();
  end_date    = query->value("end_date").toDate();
  cost        = query->value("cost").toFloat();
  status      = int_to_EStatus(query->value("status_id").toInt());
  files       = Record_Files(ETable::maintenances_files, id);

  dirty  = false;
  loaded = true;
}

bool Maintenance::delete_record()
{
  if (!loaded && !dirty) {
    qDebug() << "Cannot delete: maintenances not loaded.";
    return false;
  }

  if (!DB_MANAGER.get_db()->delete_record(ETable::maintenances, id)) {
    return false;
  }

  // reset
  damage_id   = -1;
  property_id = -1;
  description.clear();
  start_date = QDate();
  end_date   = QDate();
  cost       = 0.0f;
  status     = EStatus::None;
  files.delete_all_files();

  loaded = false;
  dirty  = false;
  return true;
}

int Maintenance::insert_record()
{
  int result = DB_MANAGER.get_db()->insert_record(ETable::maintenances, false);
  if (result < 0) return -1;

  id = result;
  update_record();
  return result;
}

bool Maintenance::update_record()
{
  if (!DB_MANAGER.get_db()->find_record(ETable::maintenances, id)) {
    qDebug() << "Update impossible, id (" << id << ") doesn't exist";
    return false;
  }

  bool issues[] = {
      DB_MANAGER.get_db()->update_record(ETable::maintenances, id, "damage_id", damage_id),
      DB_MANAGER.get_db()->update_record(ETable::maintenances, id, "property_id", property_id),
      DB_MANAGER.get_db()->update_record(ETable::maintenances, id, "description", description),
      DB_MANAGER.get_db()->update_record(ETable::maintenances, id, "start_date", start_date),
      DB_MANAGER.get_db()->update_record(ETable::maintenances, id, "end_date", end_date),
      DB_MANAGER.get_db()->update_record(ETable::maintenances, id, "cost", cost),
      DB_MANAGER.get_db()->update_record(ETable::maintenances, id, "status", static_cast<int>(status)),
  };

  for (bool success : issues) {
    if (!success) {
      qDebug() << "Update failed for maintenances id" << id;
      return false;
    }
  }

  loaded = true;
  dirty  = false;
  return true;
}
