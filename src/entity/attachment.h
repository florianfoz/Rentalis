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
  qsizetype col_id;
  QString   path;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"table", &Attachment::table),   //
        field(u"col_id", &Attachment::col_id), //
        field(u"path", &Attachment::path),     //
    };
  }

  bool operator==(const Attachment& other) const noexcept
  {
    return table == other.table &&   //
           col_id == other.col_id && //
           path == other.path;
  }
};

#endif // ATTACHMENT