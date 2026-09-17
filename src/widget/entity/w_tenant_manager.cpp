#include "widget/entity/w_tenant_manager.h"

#include "base.h"
#include "database/manager.h"
#include "ui_w_tenant_manager.h"
#include "widget/entity/w_tenant.h"
#include "widget/entity/w_tenant_creator.h"

#include <QMessageBox>
#include <QSqlQuery>


W_Tenant_Manager::W_Tenant_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Tenant_Manager)
{
  ui->setupUi(this);

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable table) {
    if (table == ETable::Tenant || table == ETable::NONE) refresh();
  });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

W_Tenant_Manager::~W_Tenant_Manager()
{
  delete ui;
}

void W_Tenant_Manager::refresh()
{
  ui->tile_view->clear();

  for (auto id : Tenant::all_records_id()) {
    auto* w = new W_Tenant(id);
    ui->tile_view->add_widget(w);
  }
}

void W_Tenant_Manager::on_b_new_clicked()
{
  auto cre = W_Tenant_Creator();
  cre.setModal(true);
  cre.exec();
}
