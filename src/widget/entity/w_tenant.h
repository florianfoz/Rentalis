#ifndef W_TENANT_H
#define W_TENANT_H

#include "forward.h"

#include <QWidget>


namespace Ui
{
class W_Tenant;
}

class W_Tenant : public QWidget
{
  Q_OBJECT

public:
  explicit W_Tenant(qsizetype id);
  ~W_Tenant();

  qsizetype id = INVALID_ID;
  bool      check_data();
  void      refresh();
private slots:
  void on_b_edit_clicked();

  void on_b_delete_clicked();

  void enterEvent(QEnterEvent* event) override;

  void leaveEvent(QEvent* event) override;

private:
  Ui::W_Tenant* ui;
};

#endif // W_TENANT_H
