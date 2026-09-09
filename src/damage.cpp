#include "damage.h"

#include "base.h"

#include <QSqlQuery>

void Damage::ensure_loaded()
{
  if (loaded || dirty) return;

  if (!DB_MANAGER.is_valid()) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::damages, id);

  if (!query->next()) {
    qDebug() << "damages id (" << id << ") is invalid !";
    dirty  = true;
    loaded = false;
    return;
  }

  property_id   = query->value("property_id").toInt();
  description   = query->value("description").toString();
  reported_date = query->value("reported_date").toDate();
  severity      = int_to_EServerity(query->value("severity_id").toInt());
  cost          = query->value("cost").toFloat();
  files         = Record_Files(ETable::damages_files, id);

  dirty  = false;
  loaded = true;
}

bool Damage::delete_record()
{
  if (!loaded && !dirty) {
    qDebug() << "Cannot delete: damages not loaded.";
    return false;
  }

  if (!DB_MANAGER.get_db()->delete_record(ETable::damages, id)) {
    return false;
  }

  // reset
  property_id = -1;
  description.clear();
  reported_date = QDate();
  cost          = 0.0f;
  severity      = ESeverity::None;
  files.delete_all_files();

  loaded = false;
  dirty  = false;
  return true;
}

int Damage::insert_record()
{
  if (loaded) {
    qWarning() << "Cannot insert record: the damage is already loaded";
    return -1;
  }

  int result = DB_MANAGER.get_db()->insert_record(ETable::damages, false);
  if (result < 0) return -1;

  id = result;
  update_record();
  return result;
}

bool Damage::update_record()
{
  if (!DB_MANAGER.get_db()->find_record(ETable::damages, id)) {
    qWarning() << "Update impossible, id (" << id << ") doesn't exist";
    return false;
  }

  bool issues[] = {
      DB_MANAGER.get_db()->update_record(ETable::damages, id, "property_id", property_id),
      DB_MANAGER.get_db()->update_record(ETable::damages, id, "description", description),
      DB_MANAGER.get_db()->update_record(ETable::damages, id, "repoted_date", reported_date),
      DB_MANAGER.get_db()->update_record(ETable::damages, id, "cost", cost),
      DB_MANAGER.get_db()->update_record(ETable::damages, id, "severity_id", static_cast<int>(severity)),
  };

  for (bool success : issues) {
    if (!success) {
      qCritical() << "Update failed for damages id" << id;
      return false;
    }
  }

  loaded = true;
  dirty  = false;
  return true;
}
