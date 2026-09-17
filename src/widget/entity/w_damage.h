#ifndef W_DAMAGE_H
#define W_DAMAGE_H

#include "forward.h"

#include <QWidget>

namespace Ui
{
class W_Damage;
}

class W_Damage : public QWidget
{
  Q_OBJECT

public:
  explicit W_Damage(qsizetype id);
  ~W_Damage();

  qsizetype id = INVALID_ID;

  void refresh();

private:
  Ui::W_Damage* ui;
};

#endif // W_DAMAGE_H
