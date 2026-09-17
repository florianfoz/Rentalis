#ifndef W_DAMAGE_CREATOR_H
#define W_DAMAGE_CREATOR_H

#include "entity/damage.h"

#include <QDialog>


namespace Ui
{
class W_Damage_Creator;
}

class W_Damage_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Damage_Creator(qsizetype id = INVALID_ID);
  ~W_Damage_Creator();

  Damage damage;

  void clear();
  void populate_ui();
  void inject_data();

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::W_Damage_Creator* ui;
};

#endif // W_DAMAGE_CREATOR_H
