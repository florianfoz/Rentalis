#ifndef W_MAINTENANCE_CREATOR_H
#define W_MAINTENANCE_CREATOR_H

#include <QDialog>

class W_Maintenance_Manager;
class Maintenance;

namespace Ui
{
class W_Maintenance_Creator;
}

class W_Maintenance_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Maintenance_Creator(W_Maintenance_Manager* manager, int id = -1);
  ~W_Maintenance_Creator();

  Maintenance*           maintenance = nullptr;
  W_Maintenance_Manager* manager     = nullptr;

  void populate_ui();
  void clear();
  void inject_data();
  void update_ui();

private:
  Ui::W_Maintenance_Creator* ui;
};

#endif // W_MAINTENANCE_CREATOR_H
