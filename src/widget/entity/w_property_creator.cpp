#include "widget/entity/w_property_creator.h"

#include "base.h"
#include "database/manager.h"
#include "entity/property.h"
#include "ui_w_property_creator.h"

#include <QFileDialog>
#include <QPushButton>

W_Property_Creator::W_Property_Creator(qsizetype id)
  : QDialog(nullptr)
  , property(Property::read_record(id))
  , ui(new Ui::W_Property_Creator)
{
  ui->setupUi(this);

  if (id == -1)
    setWindowTitle(tr("Property Creation"));
  else
    setWindowTitle(tr("Property Edition"));

  populate_ui();
}

W_Property_Creator::~W_Property_Creator()
{
  delete ui;
}

void W_Property_Creator::populate_ui()
{
  clear();
  auto count = 0;
  for (const auto& type : EPropertyType_names) ui->cb_type->addItem(type, count++);

  count = 0;
  for (const auto& cond : ECondition_names) ui->cb_condition->addItem(cond, count++);

  count = 0;
  for (const auto& energy : EEnergy_names) ui->cb_energy->addItem(energy, count++);

  if (property) {
    int type_index = ui->cb_type->findData(static_cast<int>(property.property_type));
    if (type_index >= 0) ui->cb_type->setCurrentIndex(type_index);

    int cond_index = ui->cb_condition->findData(static_cast<int>(property.condition));
    if (cond_index >= 0) ui->cb_condition->setCurrentIndex(cond_index);

    int energy_index = ui->cb_energy->findData(static_cast<int>(property.energy));
    if (energy_index >= 0) ui->cb_energy->setCurrentIndex(energy_index);

    ui->le_country->setText(property.country);
    ui->le_name->setText(property.name);
    ui->le_address->setText(property.address);
    ui->le_city->setText(property.city);
    ui->le_postal_code->setText(property.postal_code);
    ui->le_location_number->setText(property.location_number);
    ui->dsb_surface_size->setValue(property.surface);
  }
}

void W_Property_Creator::clear()
{
  ui->cb_type->clear();
  ui->cb_condition->clear();
  ui->cb_energy->clear();

  ui->le_name->clear();
  ui->le_city->clear();
  ui->le_country->clear();
  ui->l_icon->clear();
  ui->le_address->clear();
  ui->le_postal_code->clear();
  ui->le_location_number->clear();
  ui->dsb_surface_size->setValue(0);
}

void W_Property_Creator::inject_data()
{
  property.property_type   = static_cast<EPropertyType>(ui->cb_type->currentData().toInt());
  property.condition       = static_cast<ECondition>(ui->cb_condition->currentData().toInt());
  property.energy          = static_cast<EEnergy>(ui->cb_energy->currentData().toInt());
  property.country         = ui->le_country->text();
  property.city            = ui->le_city->text();
  property.name            = ui->le_name->text();
  property.address         = ui->le_address->text();
  property.postal_code     = ui->le_postal_code->text();
  property.location_number = ui->le_location_number->text();
  property.surface         = ui->dsb_surface_size->value();
}

void W_Property_Creator::on_buttonBox_accepted()
{
  inject_data();
  (void)property.save_record();
  close();
}


void W_Property_Creator::on_buttonBox_rejected()
{
  close();
}

void W_Property_Creator::on_b_icon_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Select icon image"), "",
                                                  tr("Images (*.png *.jpg *.jpeg *.bmp *.gif);;All files (*)"));

  if (!fileName.isEmpty()) {
    icon_type = QFileInfo(fileName).suffix();

    QPixmap pix(fileName);
    if (!pix.isNull()) {
      ui->l_icon->setPixmap(pix.scaled(ui->l_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
      qDebug() << "Impossible to load image :" << fileName;
    }
  }
}
