#ifndef W_TENANT_CREATOR_H
#define W_TENANT_CREATOR_H

#include "entity/tenant.h"

#include <QDialog>

class Tenant;

namespace Ui
{
class W_Tenant_Creator;
}

class W_Tenant_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Tenant_Creator(qsizetype id = INVALID_ID);
  ~W_Tenant_Creator();

  Tenant tenant;

  QString icon_type;

  void clear();
  void populate_ui();
  void inject_data();

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

  void on_b_icon_clicked();

  void on_cb_type_currentIndexChanged(int index);

  void on_le_phone_textEdited(const QString& arg1);

private:
  Ui::W_Tenant_Creator* ui;
};

#endif // W_TENANT_CREATOR_H
