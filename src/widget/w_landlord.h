#ifndef W_LANDLORD_H
#define W_LANDLORD_H

#include <QWidget>

class W_Landlord_Manager;

namespace Ui
{
class W_Landlord;
}

class W_Landlord : public QWidget
{
  Q_OBJECT

public:
  explicit W_Landlord(W_Landlord_Manager* manager, int id);
  ~W_Landlord();

  W_Landlord_Manager* manager;
  int                 id = -1;

  void refresh();
private slots:
  void on_b_edit_clicked();

  void on_b_delete_clicked();

  void enterEvent(QEnterEvent* event) override;

  void leaveEvent(QEvent* event) override;

private:
  Ui::W_Landlord* ui;
};

#endif // W_LANDLORD_H
