#include "widget/w_maintenance_manager.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "ui_w_maintenance_manager.h"
#include "widget/w_maintenance.h"
#include "widget/w_maintenance_creator.h"

W_Maintenance_Manager::W_Maintenance_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Maintenance_Manager)
{
  ui->setupUi(this);


  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

void W_Maintenance_Manager::refresh()
{
  ui->tile_view->clear();

  if (auto query = Database_Manager::current_database()->all_records(ETable::Maintenance)) {
    while (query->next()) {
      int   id          = query->value("maintenance_id").toInt();
      auto* maintenance = new W_Maintenance(this, id);
      ui->tile_view->add_widget(maintenance);
    }
  }
}

W_Maintenance_Manager::~W_Maintenance_Manager()
{
  delete ui;
}

void W_Maintenance_Manager::on_b_new_maintenance_clicked()
{
  auto* w_creator = new W_Maintenance_Creator(this, -1);
  w_creator->setModal(true);
  w_creator->exec();
}
