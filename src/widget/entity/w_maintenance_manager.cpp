#include "widget/entity/w_maintenance_manager.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "ui_w_maintenance_manager.h"
#include "widget/entity/w_maintenance.h"
#include "widget/entity/w_maintenance_creator.h"

W_Maintenance_Manager::W_Maintenance_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Maintenance_Manager)
{
  ui->setupUi(this);


  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable table) {
    if (table == ETable::Maintenance || table == ETable::NONE) refresh();
  });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

void W_Maintenance_Manager::refresh()
{
  ui->tile_view->clear();

  for (const auto& id : Maintenance::all_records_id()) {
    auto* w = new W_Maintenance(id);
    ui->tile_view->add_widget(w);
  }
}

W_Maintenance_Manager::~W_Maintenance_Manager()
{
  delete ui;
}

void W_Maintenance_Manager::on_b_new_maintenance_clicked()
{
  auto* w = new W_Maintenance_Creator();
  w->setModal(true);
  w->exec();
}
