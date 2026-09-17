#include "entity/tenant.h"

#include "database/database.h"
#include "database/manager.h"

bool Tenant::is_used(qsizetype tenant_id)
{
  QSqlQuery query(Database_Manager::current_sql());
  query.prepare(R"(
            SELECT
                EXISTS(SELECT 1 FROM rent WHERE rent.tenant_id = :tenant_id)
                OR EXISTS(SELECT 1 FROM receipt WHERE receipt.tenant_id = :tenant_id)
                OR EXISTS(SELECT 1 FROM invoice WHERE invoice.tenant_id = :tenant_id)
                AS used;
        )");
  query.bindValue(":tenant_id", tenant_id);

  Database_Manager::current_recorder()->query_check(&query, "EXISTS on tenant_id: " + QString::number(tenant_id));
  query.next();

  return query.value("used").toInt() > 0;
}