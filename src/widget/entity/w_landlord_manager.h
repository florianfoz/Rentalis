#ifndef W_LANDLORD_MANAGER_H
#define W_LANDLORD_MANAGER_H

#include <QWidget>

namespace Ui
{
class W_Landlord_Manager;
}

class W_Landlord_Manager : public QWidget
{
  Q_OBJECT

public:
  explicit W_Landlord_Manager(QWidget* parent = nullptr);
  ~W_Landlord_Manager();

  void refresh();
private slots:
  void on_b_new_clicked();

private:
  Ui::W_Landlord_Manager* ui;
};

#endif // W_LANDLORD_MANAGER_H
