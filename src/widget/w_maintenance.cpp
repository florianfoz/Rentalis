#include "widget/w_maintenance.h"

#include "base.h"
#include "entities/maintenance.h"
#include "entities/property.h"
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
  auto maintenance = Maintenance::read_record(id);

  if (maintenance) {
    ui->le_damage->setText(QString::number(maintenance.damage_id));
    ui->le_cost->setText(ftom(maintenance.cost));
    ui->le_status->setText(EStatus_to_str(maintenance.status));
    ui->te_description->setText(maintenance.description);


    auto property = Property::read_record(id);
    if (property) {
      ui->l_property_name->setText(property.name);
    }

    ui->l_period->setText(
        tr("From %1 to %2")
            .arg(maintenance.start_date.toString("dd/MM/yyyy"), maintenance.end_date.toString("dd/MM/yyyy")));
  }
}

W_Maintenance::~W_Maintenance()
{
  delete ui;
}


void W_Maintenance::on_b_edit_clicked()
{
  auto* w_creator = new W_Maintenance_Creator(nullptr, id);
  w_creator->setModal(true);
  w_creator->exec();
}


void W_Maintenance::on_b_delete_clicked()
{
  auto maintenance = Maintenance::read_record(id);
  maintenance.delete_record();
}
