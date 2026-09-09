#ifndef W_PROPERTY_ROOM_CREATOR_H
#define W_PROPERTY_ROOM_CREATOR_H

#include <QWidget>

namespace Ui
{
class W_Property_Room_Creator;
}

class W_Property_Room_Creator : public QWidget
{
  Q_OBJECT

public:
  explicit W_Property_Room_Creator(QWidget* parent = nullptr);
  ~W_Property_Room_Creator();

private:
  Ui::W_Property_Room_Creator* ui;
};

#endif // W_PROPERTY_ROOM_CREATOR_H
