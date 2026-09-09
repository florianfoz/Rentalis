#ifndef DAMAGE_H
#define DAMAGE_H

#include "record_files.h"

#include <QDate>
#include <QString>

enum class ESeverity;

class Damage
{
public:
  Damage(int id, bool lazy = false)
    : id(id)
    , loaded(false)
  {
    if (!lazy) ensure_loaded();
  }

private:
  void ensure_loaded();

public:
  // no copy
  Damage(const Damage&)            = delete;
  Damage& operator=(const Damage&) = delete;
  Damage(Damage&&)                 = default;
  Damage& operator=(Damage&&)      = default;

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
  [[nodiscard]] QDate get_reported_date()
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
  void set_reported_date(const QDate& _reported_date)
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
  QDate        reported_date;
  ESeverity    severity;
  float        cost = 0.0f;
  Record_Files files;
};

#endif // DAMAGE_H
