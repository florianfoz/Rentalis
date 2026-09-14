#include "widget/w_damage.h"

#include "base.h"
#include "entities/damage.h"
#include "entities/property.h"
#include "ui_w_damage.h"

W_Damage::W_Damage(W_Damage_Manager* manager, int id)
  : manager(manager)
  , id(id)
  , ui(new Ui::W_Damage)
{
  ui->setupUi(this);

  refresh();
}

void W_Damage::refresh()
{
  auto damage = Damage::read_record(id);

  if (damage) {
    ui->le_cost->setText(ftom(damage.cost));
    ui->le_severity->setText(ESeverity_to_str(damage.severity));
    ui->te_description->setText(damage.description);

    auto property = Property::read_record(damage.property_id);
    ui->l_name->setText(property.name);
    ui->l_period->setText(damage.reported_date.toString("dd/MM/yyyy"));
  }
}

W_Damage::~W_Damage()
{
  delete ui;
}
