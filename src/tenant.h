#ifndef TENANT_H
#define TENANT_H

#include "image_buffer.h"

#include <QDate>
#include <QString>

enum class EEntityType;
enum class ETitle;

class Tenant
{
public:
  Tenant(int id, bool lazy = false)
    : id(id)
    , loaded(false)
  {
    if (!lazy) ensure_loaded();
  }

  // no copy
  Tenant(const Tenant&)            = delete;
  Tenant& operator=(const Tenant&) = delete;
  Tenant(Tenant&&)                 = default;
  Tenant& operator=(Tenant&&)      = default;

private:
  void ensure_loaded();

public:
  // getters
  [[nodiscard]] int get_id() const
  {
    return id;
  }
  [[nodiscard]] QString get_first_name()
  {
    ensure_loaded();
    return first_name;
  }
  [[nodiscard]] QString get_last_name()
  {
    ensure_loaded();
    return last_name;
  }
  [[nodiscard]] ETitle get_title()
  {
    ensure_loaded();
    return title;
  }
  [[nodiscard]] QString get_email()
  {
    ensure_loaded();
    return email;
  }
  [[nodiscard]] QString get_phone()
  {
    ensure_loaded();
    return phone;
  }
  [[nodiscard]] QDate get_birthday()
  {
    ensure_loaded();
    return birthday;
  }
  [[nodiscard]] EEntityType get_entity_type()
  {
    ensure_loaded();
    return entity_type;
  }
  [[nodiscard]] Image_Buffer get_icon()
  {
    ensure_loaded();
    return icon;
  }

  [[nodiscard]] QString get_full_name();
  [[nodiscard]] bool    is_loaded() const
  {
    return loaded;
  }
  [[nodiscard]] bool is_dirty() const
  {
    return dirty;
  }

  // setters
  void set_first_name(const QString& _first_name)
  {
    dirty      = true;
    first_name = _first_name;
  }
  void set_last_name(const QString& _last_name)
  {
    dirty     = true;
    last_name = _last_name;
  }
  void set_title(ETitle _title)
  {
    dirty = true;
    title = _title;
  }
  void set_email(const QString& _email)
  {
    dirty = true;
    email = _email;
  }
  void set_phone(const QString& _phone)
  {
    dirty = true;
    phone = _phone;
  }
  void set_birthday(const QDate& _birthday)
  {
    dirty    = true;
    birthday = _birthday;
  }
  void set_entity_type(EEntityType _entity_type)
  {
    dirty       = true;
    entity_type = _entity_type;
  }
  void set_icon(const Image_Buffer& _icon)
  {
    dirty = true;
    icon  = _icon;
  }

  // database operations
  bool delete_record();
  int  insert_record();
  bool update_record();

private:
  int id = -1;

  bool loaded = false;
  bool dirty  = false;

  QString      first_name;
  QString      last_name;
  ETitle       title;
  QString      email;
  QString      phone;
  QDate        birthday;
  EEntityType  entity_type;
  Image_Buffer icon;
};

#endif // TENANT_H
