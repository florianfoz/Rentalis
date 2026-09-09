#include "lease_agreement.h"

#include "base.h"

#include <QSqlQuery>

void Lease_Agreement::ensure_loaded()
{
  if (loaded || dirty) return;

  auto query = DB_MANAGER.get_db()->find_record(ETable::lease_agreements, id);

  if (!query->next()) {
    qDebug() << "Lease agreement id (" << id << ") is invalid !";
    dirty  = true;
    loaded = false;
    return;
  }

  property_id   = query->value("property_id").toInt();
  description   = query->value("description").toString();
  reported_date = query->value("reported_date").toDateTime();
  severity      = int_to_EServerity(query->value("serverity").toInt());
  cost          = query->value("cost").toFloat();
  files         = Record_Files(ETable::lease_agreements_files, id);
}
