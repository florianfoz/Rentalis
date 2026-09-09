#ifndef RENT_H
#define RENT_H

#include "database.h"

class Rent
{
public:
  Rent(int id, bool lazy = false)
    : id(id)
    , loaded(false)
  {
    if (!lazy) ensure_loaded();
  }

  [[nodiscard]] static Rent load_from_tenant(int _tenant_id, const QDate& start_date);
  [[nodiscard]] static Rent load_from_property(int _property_id, const QDate& start_date);

private:
  void ensure_loaded();

public:
  // getters
  [[nodiscard]] int get_id() const
  {
    return id;
  }
  [[nodiscard]] QDate get_date()
  {
    ensure_loaded();
    return date;
  }
  [[nodiscard]] float get_rent()
  {
    ensure_loaded();
    return rent;
  }
  [[nodiscard]] float get_housing_aid()
  {
    ensure_loaded();
    return housing_aid;
  }
  [[nodiscard]] float get_charge()
  {
    ensure_loaded();
    return charge;
  }
  [[nodiscard]] float get_household_waste()
  {
    ensure_loaded();
    return household_waste;
  }
  [[nodiscard]] int get_property_id()
  {
    ensure_loaded();
    return property_id;
  }
  [[nodiscard]] int get_tenant_id()
  {
    ensure_loaded();
    return tenant_id;
  }
  [[nodiscard]] QString get_comment()
  {
    ensure_loaded();
    return comment;
  }

  [[nodiscard]] bool is_loaded()
  {
    return loaded;
  }
  [[nodiscard]] bool is_dirty()
  {
    return dirty;
  }

  // setters
  void set_date(const QDate& _date)
  {
    dirty = true;
    date  = _date;
  }
  void set_rent(float _rent)
  {
    dirty = true;
    rent  = _rent;
  }
  void set_housing_aid(float _housing_aid)
  {
    dirty       = true;
    housing_aid = _housing_aid;
  }
  void set_charge(float _charge)
  {
    dirty  = true;
    charge = _charge;
  }
  void set_household_waste(float _household_waste)
  {
    dirty           = true;
    household_waste = _household_waste;
  }
  void set_property_id(int _property_id)
  {
    dirty       = true;
    property_id = _property_id;
  }
  void set_tenant_id(int _tenant_id)
  {
    dirty     = true;
    tenant_id = _tenant_id;
  }
  void set_comment(const QString& _comment)
  {
    dirty   = true;
    comment = _comment;
  }

  // database operations
  bool delete_record();
  int  insert_record();
  bool update_record();

private:
  int id = -1;

  bool loaded = false;
  bool dirty  = false;

  QDate   date;
  float   rent            = 0.0f;
  float   housing_aid     = 0.0f;
  float   charge          = 0.0f;
  float   household_waste = 0.0f;
  int     property_id     = 0;
  int     tenant_id       = 0;
  QString comment;
};

#endif // RENT_H
