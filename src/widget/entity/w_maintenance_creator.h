#ifndef W_MAINTENANCE_CREATOR_H
#define W_MAINTENANCE_CREATOR_H


#include "entity/maintenance.h"

#include <QDialog>


namespace Ui
{
class W_Maintenance_Creator;
}

class W_Maintenance_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Maintenance_Creator(qsizetype id = INVALID_ID);
  ~W_Maintenance_Creator();

  Maintenance maintenance;

  void populate_ui();
  void clear();
  void inject_data();
  void update_ui();

private:
  Ui::W_Maintenance_Creator* ui;
};

#endif // W_MAINTENANCE_CREATOR_H
