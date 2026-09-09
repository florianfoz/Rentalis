#ifndef PROPERTY_ROOM_H
#define PROPERTY_ROOM_H

#include "image_buffer.h"

#include <QString>

template <typename T>
using uptr = std::unique_ptr<T>;

enum class ERoomType;
class Image_Buffer;

class Property_Room
{
public:
  Property_Room(int id, bool lazy = false)
    : id(id)
    , loaded(false)
  {
    if (!lazy) ensure_loaded();
  }

private:
  void ensure_loaded();

public:
  // no copy
  Property_Room(const Property_Room&)            = delete;
  Property_Room& operator=(const Property_Room&) = delete;
  Property_Room(Property_Room&&)                 = default;
  Property_Room& operator=(Property_Room&&)      = default;

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
  [[nodiscard]] ERoomType get_room_type()
  {
    ensure_loaded();
    return room_type;
  }
  [[nodiscard]] int get_surface_size()
  {
    ensure_loaded();
    return surface_size;
  }
  [[nodiscard]] bool get_is_furnished()
  {
    ensure_loaded();
    return is_furnished;
  }
  [[nodiscard]] QString get_comment()
  {
    ensure_loaded();
    return comment;
  }
  [[nodiscard]] float get_floor()
  {
    ensure_loaded();
    return floor;
  }
  [[nodiscard]] Image_Buffer& get_icon()
  {
    ensure_loaded();
    return icon;
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
  void set_room_type(ERoomType _room_type)
  {
    ensure_loaded();
    room_type = _room_type;
  }
  void set_surface_size(float _surface_size)
  {
    ensure_loaded();
    surface_size = _surface_size;
  }
  void set_is_furnished(bool _is_furnished)
  {
    ensure_loaded();
    is_furnished = _is_furnished;
  }
  void set_comment(const QString& _comment)
  {
    ensure_loaded();
    comment = _comment;
  }
  void set_floor(int _floor)
  {
    ensure_loaded();
    floor = _floor;
  }
  void set_icon(Image_Buffer& _icon)
  {
    dirty = true;
    icon  = std::move(_icon);
  }

  // database operations
  bool delete_record();
  int  insert_record();
  bool update_record();

private:
  int id = -1;

  bool loaded = false;
  bool dirty  = false;

  int          property_id  = -1;
  ERoomType    room_type    = static_cast<ERoomType>(0);
  float        surface_size = 0.0f;
  bool         is_furnished = false;
  QString      comment;
  int          floor;
  Image_Buffer icon;
};

#endif // PROPERTY_ROOM_H
