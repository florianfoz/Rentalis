#ifndef RECORD_H
#define RECORD_H

#include "forward.h"

constexpr qsizetype INVALID_ID = -1;

template <typename T>
class Record;

template <class T>
concept RecordType = std::derived_from<T, Record<T>> && requires {
  requires std::same_as<std::remove_cvref_t<decltype(T::static_table)>, ETable>;
  T::sql_fields();
};

[[nodiscard]] qsizetype create_record(ETable table);
[[nodiscard]] bool      contains_record(ETable table, qsizetype id);
[[nodiscard]] bool      delete_record(ETable table, qsizetype id, bool wmsg = false, const QString& msg = {});

template <RecordType T>
[[nodiscard]] T read_record(qsizetype id);

template <RecordType T>
[[nodiscard]] bool save_record(T* rec);

template <RecordType T>
[[nodiscard]] T from_sql(const QSqlQuery& query);

template <RecordType T>
[[nodiscard]] QList<T> all_records();

[[nodiscard]] QList<qsizetype> all_records_id(ETable table);


template <typename DERIVED>
class Record
{
public:
  [[nodiscard]]
  virtual bool is_record() const
  {
    return true;
  }

  [[nodiscard]]
  bool is_relation() const
  {
    return !is_record();
  }

  [[nodiscard]]
  static DERIVED create_record()
  {
    auto id = ::create_record(DERIVED::static_table);
    return ::read_record<DERIVED>(id);
  }

  [[nodiscard]]
  static DERIVED read_record(qsizetype id)
  {
    return ::read_record<DERIVED>(id);
  }

  [[nodiscard]]
  static QList<DERIVED> all_records()
  {
    return ::all_records<DERIVED>();
  }

  [[nodiscard]]
  static QList<qsizetype> all_records_id()
  {
    return ::all_records_id(DERIVED::static_table);
  }

  [[nodiscard]]
  bool delete_record(bool wmsg = false, const QString& msg = {})
  {
    if (::delete_record(DERIVED::static_table, id, wmsg, msg)) {
      id = INVALID_ID;
    }

    return id == INVALID_ID;
  }

  [[nodiscard]]
  bool save_record()
  {
    if (id == INVALID_ID) id = ::create_record(DERIVED::static_table);

    return ::save_record<DERIVED>(static_cast<DERIVED*>(this));
  }

  [[nodiscard]]
  bool is_valid() const
  {
    return ::contains_record(DERIVED::static_table, id);
  }

  explicit operator bool() const noexcept
  {
    return is_valid();
  }

  qsizetype id = INVALID_ID;
};

#endif // RECORD_H