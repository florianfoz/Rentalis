#include "widget/w_damage_creator.h"

#include "base.h"
#include "damage.h"
#include "ui_w_damage_creator.h"
#include "widget/w_damage_manager.h"

#include <QPushButton>

W_Damage_Creator::W_Damage_Creator(W_Damage_Manager* manager, int id)
  : manager(manager)
  , damage(new Damage(id))
  , ui(new Ui::W_Damage_Creator)
{
  ui->setupUi(this);

  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");

  if (id == -1)
    setWindowTitle(tr("Damage Creation"));
  else
    setWindowTitle(tr("Damage Edition"));

  populate_ui();
}

W_Damage_Creator::~W_Damage_Creator()
{
  delete ui;
}

void W_Damage_Creator::populate_ui()
{
  clear();

  if (damage->is_loaded()) {
    ui->dsb_cost->setValue(damage->get_cost());
    ui->dte_date->setDate(damage->get_reported_date());

    int property_id = ui->cb_property->findData(damage->get_property_id());
    ui->cb_property->setCurrentIndex(property_id);
    int severity_id = ui->cb_severity->findData(static_cast<int>(damage->get_severity()));
    ui->cb_severity->setCurrentIndex(severity_id);

    ui->te_description->setText(damage->get_description());
    // ui->w_images_creator->init_widget(damage->get_images());
  }
}

void W_Damage_Creator::clear()
{
  ui->te_description->clear();
  // ui->w_images_creator->clear();
  ui->cb_property->clear();
  ui->cb_severity->clear();
  ui->dsb_cost->setValue(0.0f);
  ui->dsb_cost->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);
  ui->dte_date->clear();

  for (auto elem : all_property_types) {
    ui->cb_property->addItem(EPropertyType_to_str(elem), static_cast<int>(elem));
  }

  for (auto elem : all_severity) {
    ui->cb_severity->addItem(ESeverity_to_str(elem), static_cast<int>(elem));
  }
}

void W_Damage_Creator::inject_data()
{
  damage->set_description(ui->te_description->document()->toMarkdown());
  damage->set_cost(ui->dsb_cost->value());
  damage->set_property_id(ui->cb_property->currentData().toInt());
  damage->set_severity(int_to_EServerity(ui->cb_severity->currentData().toInt()));
  damage->set_reported_date(ui->dte_date->date());
  // damage->set_images(std::unique_ptr<Images>(ui->w_images_creator->images));
}

void W_Damage_Creator::on_buttonBox_accepted()
{
  if (damage->is_loaded() && damage->is_dirty()) {
    damage->update_record();
  } else if (damage->is_dirty()) {
    damage->insert_record();
  }

  if (manager) manager->refresh();
  close();
}


void W_Damage_Creator::on_buttonBox_rejected()
{
  if (manager) manager->refresh();
  close();
}
