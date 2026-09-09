#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include "record_files.h"

#include <QDate>
#include <QString>

enum class EStatus;

class Maintenance
{
public:
  Maintenance(int id, bool lazy = false)
    : id(id)
    , loaded(false)
  {
    if (!lazy) ensure_loaded();
  }

private:
  void ensure_loaded();

public:
  // no copy
  Maintenance(const Maintenance&)            = delete;
  Maintenance& operator=(const Maintenance&) = delete;
  Maintenance(Maintenance&&)                 = default;
  Maintenance& operator=(Maintenance&&)      = default;

  // getters
  [[nodiscard]] int get_id() const
  {
    return id;
  }
  [[nodiscard]] int get_damage_id()
  {
    ensure_loaded();
    return damage_id;
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
  [[nodiscard]] QDate get_start_date()
  {
    ensure_loaded();
    return start_date;
  }
  [[nodiscard]] QDate get_end_date()
  {
    ensure_loaded();
    return end_date;
  }
  [[nodiscard]] float get_cost()
  {
    ensure_loaded();
    return cost;
  }
  [[nodiscard]] EStatus get_status()
  {
    ensure_loaded();
    return status;
  }
  [[nodiscard]] Record_Files& get_images()
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
  void set_damage_id(int _damage_id)
  {
    ensure_loaded();
    damage_id = _damage_id;
  }
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
  void set_start_date(const QDate& _start_date)
  {
    ensure_loaded();
    start_date = _start_date;
  }
  void set_end_date(const QDate& _end_date)
  {
    ensure_loaded();
    end_date = _end_date;
  }
  void set_cost(float _cost)
  {
    ensure_loaded();
    cost = _cost;
  }
  void set_status(EStatus _status)
  {
    ensure_loaded();
    status = _status;
  }

  // database operations
  bool delete_record();
  int  insert_record();
  bool update_record();

private:
  int id = -1;

  bool loaded = false;
  bool dirty  = false;

  int          damage_id   = -1;
  int          property_id = -1;
  QString      description;
  QDate        start_date;
  QDate        end_date;
  float        cost = 0.0f;
  EStatus      status;
  Record_Files files;
};

#endif // MAINTENANCE_H
