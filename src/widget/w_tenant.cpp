#include "widget/w_tenant.h"

#include "base.h"
#include "tenant.h"
#include "ui_w_tenant.h"
#include "widget/w_tenant_creator.h"
#include "widget/w_tenant_manager.h"

#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>


W_Tenant::W_Tenant(W_Tenant_Manager* manager, int id)
  : manager(manager)
  , id(id)
  , ui(new Ui::W_Tenant)
{
  ui->setupUi(this);

  refresh();
}

void W_Tenant::refresh()
{
  ui->b_delete->setHidden(true);
  ui->b_edit->setHidden(true);

  Tenant tenant(id);

  ui->l_name->setText(tenant.get_full_name());
  ui->le_email->setText(tenant.get_email());
  ui->le_birthday->setText(tenant.get_birthday().toString("dd/MM/yyyy"));
  ui->le_phone->setText(tenant.get_phone());
  if (auto pix = tenant.get_icon().get_image(); !pix.isNull())
    ui->l_icon->setPixmap(pix);
  else
    ui->l_icon->setPixmap(QPixmap("://assets/system-users.svg"));

  ui->le_type->setText(EEntityType_to_str(tenant.get_entity_type()));

  if (!is_EEntityType_is_human(tenant.get_entity_type())) ui->le_birthday->setHidden(true);
}

W_Tenant::~W_Tenant()
{
  delete ui;
}


void W_Tenant::on_b_delete_clicked()
{
  Tenant tenant(id);
  DB_MANAGER.get_db()->delete_record(ETable::tenants, id, true, QObject::tr("Tenant: %1").arg(tenant.get_full_name()));
}

void W_Tenant::enterEvent(QEnterEvent* event)
{
  ui->b_edit->setHidden(false);
  ui->b_delete->setHidden(false);
}

void W_Tenant::leaveEvent(QEvent* event)
{
  ui->b_edit->setHidden(true);
  ui->b_delete->setHidden(true);
}


void W_Tenant::on_b_edit_clicked()
{
  auto edit = W_Tenant_Creator(manager, id);
  edit.setModal(true);
  edit.exec();
}
