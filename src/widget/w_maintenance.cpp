#include "widget/w_maintenance.h"

#include "base.h"
#include "maintenance.h"
#include "property.h"
#include "ui_w_maintenance.h"
#include "widget/w_maintenance_creator.h"
#include "widget/w_maintenance_manager.h"

W_Maintenance::W_Maintenance(W_Maintenance_Manager* manager, int id)
  : ui(new Ui::W_Maintenance)
{
  ui->setupUi(this);

  refresh();
}

void W_Maintenance::refresh()
{
  Maintenance maintenance(id);

  if (maintenance.is_loaded()) {
    ui->le_damage->setText(QString::number(maintenance.get_damage_id()));
    ui->le_cost->setText(ftom(maintenance.get_cost()));
    ui->le_status->setText(EStatus_to_str(maintenance.get_status()));
    ui->te_description->setText(maintenance.get_description());


    Property property(maintenance.get_property_id());
    if (property.is_loaded()) {
      ui->l_property_name->setText(property.get_name());
    }

    ui->l_period->setText(tr("From %1 to %2")
                              .arg(maintenance.get_start_date().toString("dd/MM/yyyy"),
                                   maintenance.get_end_date().toString("dd/MM/yyyy")));
  }
}

W_Maintenance::~W_Maintenance()
{
  delete ui;
}


void W_Maintenance::on_b_edit_clicked()
{
  auto w_creator = new W_Maintenance_Creator(nullptr, id);
  w_creator->setModal(true);
  w_creator->exec();
}


void W_Maintenance::on_b_delete_clicked()
{
  Maintenance maintenance(id);
  maintenance.delete_record();
}
