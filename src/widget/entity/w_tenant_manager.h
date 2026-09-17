#ifndef W_TENANT_MANAGER_H
#define W_TENANT_MANAGER_H

#include <QWidget>

namespace Ui
{
class W_Tenant_Manager;
}

class W_Tenant_Manager : public QWidget
{
  Q_OBJECT

public:
  explicit W_Tenant_Manager(QWidget* parent = nullptr);
  ~W_Tenant_Manager();

  void refresh();
private slots:
  void on_b_new_clicked();

private:
  Ui::W_Tenant_Manager* ui;
};

#endif // W_TENANT_MANAGER_H
