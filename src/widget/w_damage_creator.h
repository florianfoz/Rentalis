#ifndef W_DAMAGE_CREATOR_H
#define W_DAMAGE_CREATOR_H

#include <QDialog>

class W_Damage_Manager;
class Damage;

namespace Ui
{
class W_Damage_Creator;
}

class W_Damage_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Damage_Creator(W_Damage_Manager* manager = nullptr, int id = -1);
  ~W_Damage_Creator();

  W_Damage_Manager* manager = nullptr;
  Damage*           damage  = nullptr;

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
