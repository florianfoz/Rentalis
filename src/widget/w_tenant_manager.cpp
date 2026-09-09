#include "widget/w_tenant_manager.h"

#include "base.h"
#include "ui_w_tenant_manager.h"
#include "widget/w_tenant.h"
#include "widget/w_tenant_creator.h"

#include <QMessageBox>
#include <QSqlQuery>


W_Tenant_Manager::W_Tenant_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Tenant_Manager)
{
  ui->setupUi(this);

  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

W_Tenant_Manager::~W_Tenant_Manager()
{
  delete ui;
}

void W_Tenant_Manager::refresh()
{
  ui->tile_view->clear();

  QSqlQuery query(DB_MANAGER.get_sql_db());
  if (query.exec("SELECT * FROM tenants")) {
    while (query.next()) {
      int  id     = query.value("tenant_id").toInt();
      auto tenant = new W_Tenant(this, id);
      ui->tile_view->add_widget(tenant);
    }
  } else {
    QMessageBox::critical(this, tr("Search Failed"), tr("The tenants search failed !"));
  }
}

void W_Tenant_Manager::on_b_new_clicked()
{
  auto cre = W_Tenant_Creator(this, -1);
  cre.setModal(true);
  cre.exec();
}
