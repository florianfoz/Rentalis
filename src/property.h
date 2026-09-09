#ifndef PROPERTY_H
#define PROPERTY_H

#include "image_buffer.h"
#include "record_files.h"

#include <QString>


enum class EPropertyType;
enum class ECondition;
enum class EEnergy;

class Property
{
public:
  Property(int id, bool lazy = false)
    : id(id)
    , loaded(false)
  {
    if (!lazy) ensure_loaded();
  }

private:
  void ensure_loaded();

public:
  // no copy
  Property(const Property&)            = delete;
  Property& operator=(const Property&) = delete;
  Property(Property&&)                 = default;
  Property& operator=(Property&&)      = default;

  // getters
  [[nodiscard]] int get_id() const
  {
    return id;
  }
  [[nodiscard]] QString get_name()
  {
    ensure_loaded();
    return name;
  }
  [[nodiscard]] QString get_address()
  {
    ensure_loaded();
    return address;
  }
  [[nodiscard]] QString get_country()
  {
    ensure_loaded();
    return country;
  }
  [[nodiscard]] QString get_location_number()
  {
    ensure_loaded();
    return location_number;
  }
  [[nodiscard]] Image_Buffer& get_icon()
  {
    ensure_loaded();
    return icon;
  }
  [[nodiscard]] QString get_city()
  {
    ensure_loaded();
    return city;
  }
  [[nodiscard]] QString get_postal_code()
  {
    ensure_loaded();
    return postal_code;
  }
  [[nodiscard]] EPropertyType get_property_type()
  {
    ensure_loaded();
    return property_type;
  }
  [[nodiscard]] int get_surface()
  {
    ensure_loaded();
    return surface;
  }
  [[nodiscard]] ECondition get_condition()
  {
    ensure_loaded();
    return condition;
  }
  [[nodiscard]] EEnergy get_energy()
  {
    ensure_loaded();
    return energy;
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
  void set_name(const QString& _name)
  {
    dirty = true;
    name  = _name;
  }
  void set_country(const QString& _country)
  {
    dirty   = true;
    country = _country;
  }
  void set_address(const QString& _address)
  {
    dirty   = true;
    address = _address;
  }
  void set_location_number(const QString& _location_number)
  {
    dirty           = true;
    location_number = _location_number;
  }
  void set_icon(const Image_Buffer& _icon)
  {
    dirty = true;
    icon  = _icon;
  }
  void set_city(const QString& _city)
  {
    dirty = true;
    city  = _city;
  }
  void set_postal_code(const QString& _postal_code)
  {
    dirty       = true;
    postal_code = _postal_code;
  }
  void set_property_type(EPropertyType _property_type)
  {
    dirty         = true;
    property_type = _property_type;
  }
  void set_surface(int _surface)
  {
    dirty   = true;
    surface = _surface;
  }
  void set_condition(ECondition _condition)
  {
    dirty     = true;
    condition = _condition;
  }
  void set_energy(EEnergy _energy)
  {
    dirty  = true;
    energy = _energy;
  }

  // database operations
  bool delete_record();
  int  insert_record();
  bool update_record();

private:
  int id = -1;

  bool loaded = false;
  bool dirty  = false;

  QString       name;
  QString       address;
  QString       country;
  QString       location_number;
  Image_Buffer  icon;
  QString       city;
  QString       postal_code;
  EPropertyType property_type;
  int           surface = 9;
  ECondition    condition;
  EEnergy       energy;
  Record_Files  files;
};

#endif // PROPERTY_H
