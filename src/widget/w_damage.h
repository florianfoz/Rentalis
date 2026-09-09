#ifndef W_DAMAGE_H
#define W_DAMAGE_H

#include <QWidget>

class W_Damage_Manager;
class Damage;

namespace Ui
{
class W_Damage;
}

class W_Damage : public QWidget
{
  Q_OBJECT

public:
  explicit W_Damage(W_Damage_Manager* manager, int id);
  ~W_Damage();

  W_Damage_Manager* manager = nullptr;
  int               id      = -1;

  void refresh();

private:
  Ui::W_Damage* ui;
};

#endif // W_DAMAGE_H
