#ifndef FIELD_H
#define FIELD_H

#include "forward.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <tuple>
#include <type_traits>
#include <utility>


void emit_on_record_saved(ETable table);

template <typename T>
struct SqlConverter;

template <typename Record, typename Member>
struct SqlField {
  QStringView name;
  Member Record::* member;

  void read(Record& record, const QSqlQuery& query) const
  {
    record.*member = SqlConverter<Member>::from_sql(query.value(name));
  }

  void bind(QSqlQuery& query, const Record& record) const
  {
    query.bindValue(":" + name, SqlConverter<Member>::to_sql(record.*member));
  }
};
template <typename Record, typename F>
struct SqlCustomField {
  F reader;

  void read_from(Record& record, const QSqlQuery& sql) const
  {
    reader(record, sql);
  }
};

template <typename Record, typename F>
constexpr auto custom_field(F&& reader)
{
  return SqlCustomField<Record, std::decay_t<F>>{std::forward<F>(reader)};
}

template <typename T>
struct SqlConverter {
  static T from_sql(const QVariant& value)
  {
    return value.value<T>();
  }

  static QVariant to_sql(const T& value)
  {
    return QVariant::fromValue(value);
  }
};

template <typename Record>
void read_sql_fields(Record& record, const QSqlQuery& query)
{
  std::apply([&](const auto&... fields) { (fields.read(record, query), ...); }, Record::sql_fields());
}

template <typename Record>
QString make_update_sql()
{
  QStringList assignments;

  std::apply([&](const auto&... fields) { (assignments.append(QString("%1 = :%1").arg(fields.name)), ...); },
             Record::sql_fields());

  return QString("UPDATE %1 SET %2 WHERE id = :id").arg(ETable_to_str(Record::static_table), assignments.join(", "));
}

template <typename Record>
bool save_record(QSqlQuery& query, const Record& record)
{
  query.prepare(make_update_sql<Record>());

  std::apply([&](const auto&... fields) { (fields.bind(query, record), ...); }, Record::sql_fields());

  query.bindValue(":id", record.id);

  if (!query.exec()) return false;
  ::emit_on_record_saved(Record::static_table);
  return true;
}

template <typename Record>
QString make_select_sql()
{
  QStringList columns;

  std::apply([&](const auto&... fields) { (columns.append(QString(fields.name)), ...); }, Record::sql_fields());

  return QString("SELECT id, %1 FROM %2 WHERE id = :id").arg(columns.join(", "), ETable_to_str(Record::static_table));
}

template <typename Record, typename Member>
constexpr auto field(QStringView name, Member Record::* member)
{
  return SqlField<Record, Member>{name, member};
}

#endif // FIELD_H