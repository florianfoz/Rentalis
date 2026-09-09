#include "image_buffer.h"

#include <QBuffer>
#include <QPixmap>

void Image_Buffer::set_image(const QByteArray& _image, const QString& _image_type)
{
  if (_image.isEmpty()) {
    clear();
    return;
  }

  if (!image.loadFromData(_image, _image_type.toStdString().c_str())) {
    clear();
    return;
  }

  image_type = _image_type;
  blob       = _image;
}

void Image_Buffer::set_image(const QPixmap& _image, const QString& _image_type)
{
  image      = _image;
  image_type = _image_type;

  if (_image.isNull()) {
    blob.clear();
    return;
  }

  QByteArray byteArray;
  QBuffer    buffer(&byteArray);
  buffer.open(QIODevice::WriteOnly);

  if (!image.toImage().save(&buffer, _image_type.toUpper().toUtf8().constData())) {
    blob.clear();
    return;
  }

  blob = byteArray;
}

void Image_Buffer::clear()
{
  blob.clear();
  image = QPixmap();
  image_type.clear();
}
