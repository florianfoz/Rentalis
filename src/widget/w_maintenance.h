#ifndef W_MAINTENANCE_H
#define W_MAINTENANCE_H

#include <QWidget>

class W_Maintenance_Manager;
class Maintenace;

namespace Ui
{
class W_Maintenance;
}

class W_Maintenance : public QWidget
{
  Q_OBJECT

public:
  explicit W_Maintenance(W_Maintenance_Manager* manager, int id);
  ~W_Maintenance();

  W_Maintenance_Manager* manager;
  int                    id = -1;

  void refresh();
private slots:
  void on_b_edit_clicked();

  void on_b_delete_clicked();

private:
  Ui::W_Maintenance* ui;
};

#endif // W_MAINTENANCE_H
