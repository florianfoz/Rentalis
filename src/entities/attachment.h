#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include "record/field/field.h"
#include "record/record.h"

class Attachment : public Record<Attachment>
{
public:
  static constexpr ETable static_table = ETable::Attachment;

  static QList<Attachment> find_attachments(ETable table, qsizetype target_record);

  ETable    table;
  qsizetype id;
  QString   path;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"table", &Attachment::table), //
        field(u"id", &Attachment::id),       //
        field(u"path", &Attachment::path),   //
    };
  }
};

#endif // ATTACHMENT