#ifndef LEASE_AGREEMENT_H
#define LEASE_AGREEMENT_H

#include "record_files.h"

#include <QDate>
#include <QString>


enum class ESeverity;

class Lease_Agreement
{
public:
  Lease_Agreement(int id, bool lazy = false)
    : id(id)
    , loaded(false)
  {
    if (!lazy) ensure_loaded();
  }

private:
  void ensure_loaded();

public:
  // no copy
  Lease_Agreement(const Lease_Agreement&)            = delete;
  Lease_Agreement& operator=(const Lease_Agreement&) = delete;
  Lease_Agreement(Lease_Agreement&&)                 = default;
  Lease_Agreement& operator=(Lease_Agreement&&)      = default;

  // getters
  [[nodiscard]] int get_id() const
  {
    return id;
  }
  [[nodiscard]] int get_property_id()
  {
    ensure_loaded();
    return property_id;
  }
  [[nodiscard]] QString get_description()
  {
    ensure_loaded();
    return description;
  }
  [[nodiscard]] QDateTime get_reported_date()
  {
    ensure_loaded();
    return reported_date;
  }
  [[nodiscard]] float get_cost()
  {
    ensure_loaded();
    return cost;
  }
  [[nodiscard]] ESeverity get_severity()
  {
    ensure_loaded();
    return severity;
  }
  [[nodiscard]] Record_Files& get_files()
  {
    ensure_loaded();
    return files;
  }

  [[nodiscard]] bool is_loaded() const
  {
    return loaded;
  }
  [[nodiscard]] bool is_dirty() const
  {
    return dirty;
  }

  // setters
  void set_property_id(int _property_id)
  {
    ensure_loaded();
    property_id = _property_id;
  }
  void set_description(const QString& _description)
  {
    ensure_loaded();
    description = _description;
  }
  void set_reported_date(const QDateTime& _reported_date)
  {
    ensure_loaded();
    reported_date = _reported_date;
  }
  void set_severity(ESeverity _severity)
  {
    ensure_loaded();
    severity = _severity;
  }
  void set_cost(float _cost)
  {
    ensure_loaded();
    cost = _cost;
  }

  // database operations
  bool delete_record();
  int  insert_record();
  bool update_record();

private:
  int id = -1;

  bool loaded = false;
  bool dirty  = false;

  int          property_id = -1;
  QString      description;
  QDateTime    reported_date;
  ESeverity    severity;
  float        cost = 0.0f;
  Record_Files files;
};

#endif // LEASE_AGREEMENT_H
