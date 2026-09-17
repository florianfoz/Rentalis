#ifndef W_PROPERTY_MANAGER_H
#define W_PROPERTY_MANAGER_H

#include <QWidget>

namespace Ui
{
class W_Property_Manager;
}

class W_Property_Manager : public QWidget
{
  Q_OBJECT

public:
  explicit W_Property_Manager(QWidget* parent = nullptr);
  ~W_Property_Manager();

  void refresh();
private slots:
  void on_b_new_clicked();

private:
  Ui::W_Property_Manager* ui;
};

#endif // W_PROPERTY_MANAGER_H
