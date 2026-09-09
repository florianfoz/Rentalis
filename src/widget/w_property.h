#ifndef W_PROPERTY_H
#define W_PROPERTY_H

#include <QWidget>

class W_Property_Manager;

namespace Ui
{
class W_Property;
}

class W_Property : public QWidget
{
  Q_OBJECT

public:
  explicit W_Property(W_Property_Manager* manager, int id);
  ~W_Property();

  W_Property_Manager* manager;
  int                 id = -1;

  void refresh();
private slots:
  void on_b_edit_clicked();

  void on_b_delete_clicked();

private:
  Ui::W_Property* ui;
};

#endif // W_PROPERTY_H
