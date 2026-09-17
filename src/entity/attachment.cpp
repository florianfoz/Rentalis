#include "entity/attachment.h"

#include "database/manager.h"

QList<Attachment> Attachment::find_attachments(ETable table, qsizetype target_record)
{
  Record<void*> rec;
  switch (table) {
  case ETable::Attachment:
  case ETable::NONE:       assert(false && "invalid table type");
  default:                 break;
  }

  QSqlQuery query(Database_Manager::current_sql());
  query.prepare(R"(
SELECT * 
FROM attachment 
INNER JOIN :table
    ON attachment.id = :table.id;
    )");
  query.bindValue(":table", ETable_to_str(table));

  QList<Attachment> found;

  if (!query.exec()) return {};

  while (query.next()) {
    found.append(Attachment::read_record(query.value("col_id").toInt()));
  }

  return found;
}
