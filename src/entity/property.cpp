#include "entity/property.h"

#include "database/database.h"
#include "database/manager.h"
#include "database/recorder.h"

bool Property::is_used(qsizetype property_id)
{
  QSqlQuery query(Database_Manager::current_sql());
  query.prepare(R"(
        SELECT
            EXISTS(SELECT 1 FROM rent WHERE rent.property_id = :property_id)
            AS used;
    )");
  query.bindValue(":property_id", property_id);

  if (!Database_Manager::current_recorder()->query_check(&query,
                                                         "EXISTS on rent.property_id: " + QString::number(property_id)))
    return false;
  query.next();

  return query.value("used").toInt() > 0;
}