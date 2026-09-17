#include "widget/entity/w_tenant.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "entity/tenant.h"
#include "ui_w_tenant.h"
#include "widget/entity/w_tenant_creator.h"
#include "widget/entity/w_tenant_manager.h"

#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>


W_Tenant::W_Tenant(qsizetype id)
  : id(id)
  , ui(new Ui::W_Tenant)
{
  ui->setupUi(this);

  refresh();
}

void W_Tenant::refresh()
{
  ui->b_delete->setHidden(true);
  ui->b_edit->setHidden(true);

  auto tenant = Tenant::read_record(id);

  ui->l_name->setText(tenant.get_full_name());
  ui->le_email->setText(tenant.email);
  ui->le_birthday->setText(tenant.birthday.toString("dd/MM/yyyy"));
  ui->le_phone->setText(tenant.phone);
  // if (auto pix = tenant.icon.image; !pix.isNull())
  //   ui->l_icon->setPixmap(pix);
  // else
  ui->l_icon->setPixmap(QPixmap("://assets/system-users.svg"));

  ui->le_type->setText(EEntityType_to_str(tenant.entity_type));

  if (!EEntityType_is_human(tenant.entity_type)) ui->le_birthday->setHidden(true);
}

W_Tenant::~W_Tenant()
{
  delete ui;
}


void W_Tenant::on_b_delete_clicked()
{
  auto tenant = Tenant::read_record(id);
  (void)tenant.delete_record(true, QObject::tr("Tenant: %1").arg(tenant.get_full_name()));
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
  auto edit = W_Tenant_Creator(id);
  edit.setModal(true);
  edit.exec();
}
