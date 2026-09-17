#include "widget/entity/w_damage_creator.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "entity/damage.h"
#include "ui_w_damage_creator.h"
#include "widget/entity/w_damage_manager.h"

#include <QPushButton>

W_Damage_Creator::W_Damage_Creator(qsizetype id)
  : damage(Damage::read_record(id))
  , ui(new Ui::W_Damage_Creator)
{
  ui->setupUi(this);

  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Apply))
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

  if (damage) {
    ui->dsb_cost->setValue(damage.cost);
    ui->dte_date->setDate(damage.reported_date);

    int property_id = ui->cb_property->findData(damage.property_id);
    ui->cb_property->setCurrentIndex(property_id);
    int severity_id = ui->cb_severity->findData(static_cast<int>(damage.severity));
    ui->cb_severity->setCurrentIndex(severity_id);

    ui->te_description->setText(damage.description);
    // ui->w_images_creator->init_widget(damage.images);
  }
}

void W_Damage_Creator::clear()
{
  ui->te_description->clear();
  // ui->w_images_creator->clear();
  ui->cb_property->clear();
  ui->cb_severity->clear();
  ui->dsb_cost->setValue(0.0F);
  ui->dsb_cost->setPrefix(Database_Manager::current_manifest()->currency.symbol);
  ui->dte_date->clear();

  auto count = 0;
  for (const auto& elem : EPropertyType_names) {
    ui->cb_property->addItem(elem, count++);
  }

  count = 0;
  for (const auto& elem : ESeverity_names) {
    ui->cb_severity->addItem(elem, count++);
  }
}

void W_Damage_Creator::inject_data()
{
  damage.description   = ui->te_description->document()->toMarkdown();
  damage.cost          = ui->dsb_cost->value();
  damage.property_id   = ui->cb_property->currentData().toInt();
  damage.severity      = static_cast<ESeverity>(ui->cb_severity->currentData().toInt());
  damage.reported_date = ui->dte_date->date();
  // damage.images = std::unique_ptr<Images>(ui->w_images_creator->images);
}

void W_Damage_Creator::on_buttonBox_accepted()
{
  inject_data();
  (void)damage.save_record();
  close();
}


void W_Damage_Creator::on_buttonBox_rejected()
{
  close();
}
