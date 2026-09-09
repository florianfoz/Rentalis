#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

#include <QPixmap>
#include <QString>

class Image_Buffer
{
public:
  Image_Buffer()
  {
  }
  ~Image_Buffer()
  {
    clear();
  }

  Image_Buffer(QByteArray _image, const QString& _image_type)
  {
    set_image(_image, _image_type);
  }
  Image_Buffer(const QPixmap& _image, const QString& _image_type)
  {
    set_image(_image, _image_type);
  }

  // getter
  [[nodiscard]] QPixmap get_image() const
  {
    return image;
  }
  [[nodiscard]] QByteArray get_blob() const
  {
    return blob;
  }
  [[nodiscard]] QString get_image_type() const
  {
    return image_type;
  }
  [[nodiscard]] QString get_description() const
  {
    return description;
  }
  [[nodiscard]] QString get_name() const
  {
    return name;
  }

  // setter
  void set_image(const QByteArray& _image, const QString& _image_type);
  void set_image(const QPixmap& _image, const QString& _image_type);
  void set_description(const QString& _desc)
  {
    description = _desc;
  }
  void set_name(const QString& _name)
  {
    name = _name;
  }
  void clear();

private:
  QPixmap    image;
  QByteArray blob;
  QString    image_type;
  QString    description;
  QString    name;
};

#endif // IMAGE_BUFFER_H
