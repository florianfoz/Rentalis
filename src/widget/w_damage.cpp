#include "widget/w_damage.h"

#include "base.h"
#include "damage.h"
#include "property.h"
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
  Damage damage(id);

  if (damage.is_loaded()) {
    ui->le_cost->setText(ftom(damage.get_cost()));
    ui->le_severity->setText(ESeverity_to_str(damage.get_severity()));
    ui->te_description->setText(damage.get_description());

    Property property(damage.get_property_id());
    ui->l_name->setText(property.get_name());
    ui->l_period->setText(damage.get_reported_date().toString("dd/MM/yyyy"));
  }
}

W_Damage::~W_Damage()
{
  delete ui;
}
