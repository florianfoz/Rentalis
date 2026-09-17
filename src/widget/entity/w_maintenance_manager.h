#ifndef W_MAINTENANCE_MANAGER_H
#define W_MAINTENANCE_MANAGER_H

#include <QWidget>

namespace Ui
{
class W_Maintenance_Manager;
}

class W_Maintenance_Manager : public QWidget
{
  Q_OBJECT

public:
  explicit W_Maintenance_Manager(QWidget* parent = nullptr);
  ~W_Maintenance_Manager();

  void refresh();
private slots:
  void on_b_new_maintenance_clicked();

private:
  Ui::W_Maintenance_Manager* ui;
};

#endif // W_MAINTENANCE_MANAGER_H
