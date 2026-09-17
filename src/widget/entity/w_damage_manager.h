#ifndef W_DAMAGE_MANAGER_H
#define W_DAMAGE_MANAGER_H

#include <QWidget>

namespace Ui
{
class W_Damage_Manager;
}

class W_Damage_Manager : public QWidget
{
  Q_OBJECT

public:
  explicit W_Damage_Manager(QWidget* parent = nullptr);
  ~W_Damage_Manager();

  void refresh();
private slots:
  void on_b_new_damage_clicked();

private:
  Ui::W_Damage_Manager* ui;
};

#endif // W_DAMAGE_MANAGER_H
