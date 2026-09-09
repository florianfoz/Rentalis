#ifndef PROPERTY_FEATURE_H
#define PROPERTY_FEATURE_H

#include <QString>

template <typename T>
using uptr = std::unique_ptr<T>;

enum class EFeatureType;
class Image_Buffer;

class Property_Feature
{
public:
  Property_Feature(int id, bool lazy = false)
    : id(id)
    , loaded(false)
  {
    if (!lazy) ensure_loaded();
  }

private:
  void ensure_loaded();

public:
  // no copy
  Property_Feature(const Property_Feature&)            = delete;
  Property_Feature& operator=(const Property_Feature&) = delete;
  Property_Feature(Property_Feature&&)                 = default;
  Property_Feature& operator=(Property_Feature&&)      = default;

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
  [[nodiscard]] EFeatureType get_feature_type()
  {
    ensure_loaded();
    return feature_type;
  }
  [[nodiscard]] int get_surface_size()
  {
    ensure_loaded();
    return surface_size;
  }
  [[nodiscard]] QString get_comment()
  {
    ensure_loaded();
    return comment;
  }
  [[nodiscard]] float get_is_indoor()
  {
    ensure_loaded();
    return is_indoor;
  }
  [[nodiscard]] const Image_Buffer* get_icon()
  {
    ensure_loaded();
    return icon.get();
  }
  [[nodiscard]]

  [[nodiscard]] bool
  is_loaded() const
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
  void set_feature_type(EFeatureType _feature_type)
  {
    ensure_loaded();
    feature_type = _feature_type;
  }
  void set_surface_size(float _surface_size)
  {
    ensure_loaded();
    surface_size = _surface_size;
  }
  void set_is_indoor(bool _is_indoor)
  {
    ensure_loaded();
    is_indoor = _is_indoor;
  }
  void set_comment(const QString& _comment)
  {
    ensure_loaded();
    comment = _comment;
  }
  void set_icon(uptr<Image_Buffer> _icon)
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

  int                property_id  = -1;
  EFeatureType       feature_type = static_cast<EFeatureType>(0);
  float              surface_size = 0.0f;
  bool               is_indoor    = false;
  QString            comment;
  uptr<Image_Buffer> icon;
};

#endif // PROPERTY_FEATURE_H
