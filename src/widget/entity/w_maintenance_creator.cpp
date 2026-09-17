#include "widget/entity/w_maintenance_creator.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "entity/maintenance.h"
#include "ui_w_maintenance_creator.h"
#include "widget/entity/w_maintenance_manager.h"

W_Maintenance_Creator::W_Maintenance_Creator(qsizetype id)
  : QDialog(nullptr)
  , maintenance(Maintenance::read_record(id))
  , ui(new Ui::W_Maintenance_Creator)
{
  ui->setupUi(this);

  populate_ui();
}

W_Maintenance_Creator::~W_Maintenance_Creator()
{
  delete ui;
}

void W_Maintenance_Creator::populate_ui()
{
  clear();

  if (maintenance) {
  }
}

void W_Maintenance_Creator::clear()
{
  ui->cb_damage->clear();
  ui->cb_property->clear();
  ui->cb_status->clear();
  ui->dsb_cost->setValue(0.0F);
  ui->dsb_cost->setPrefix(Database_Manager::current_manifest()->currency.symbol);
  auto current_date = QDateTime::currentDateTime();
  ui->dte_start->setDateTime(current_date);
  ui->dte_end->setDateTime(current_date.addMonths(1));
  ui->te_description->setText("");
}

void W_Maintenance_Creator::inject_data()
{
}

void W_Maintenance_Creator::update_ui()
{
}
