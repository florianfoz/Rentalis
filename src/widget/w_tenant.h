#ifndef W_TENANT_H
#define W_TENANT_H

#include <QWidget>

class W_Tenant_Manager;

namespace Ui
{
class W_Tenant;
}

class W_Tenant : public QWidget
{
  Q_OBJECT

public:
  explicit W_Tenant(W_Tenant_Manager* manager, int id);
  ~W_Tenant();

  W_Tenant_Manager* manager;
  int               id = -1;
  bool              check_data();
  void              refresh();
private slots:
  void on_b_edit_clicked();

  void on_b_delete_clicked();

  void enterEvent(QEnterEvent* event) override;

  void leaveEvent(QEvent* event) override;

private:
  Ui::W_Tenant* ui;
};

#endif // W_TENANT_H
