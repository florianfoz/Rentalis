#include "widget/w_property_creator.h"

#include "base.h"
#include "property.h"
#include "ui_w_property_creator.h"
#include "widget/w_property_manager.h"

#include <QFileDialog>
#include <QPushButton>

W_Property_Creator::W_Property_Creator(W_Property_Manager* manager, int id)
  : QDialog(manager)
  , manager(manager)
  , property(new Property(id))
  , ui(new Ui::W_Property_Creator)
{
  ui->setupUi(this);

  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");

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

  for (auto type : all_property_types) {
    ui->cb_type->addItem(EPropertyType_to_str(type), static_cast<int>(type));
  }

  for (auto cond : all_conditions) {
    ui->cb_condition->addItem(ECondition_to_str(cond), static_cast<int>(cond));
  }

  for (auto energy : all_energy) {
    ui->cb_type->addItem(EEnergy_to_str(energy), static_cast<int>(energy));
  }

  const auto locales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);

  for (auto local : locales) {
    ui->cb_country->addItem(local.nativeTerritoryName(), local.name());
  }

  if (property->is_loaded()) {
    int type_index = ui->cb_type->findData(static_cast<int>(property->get_property_type()));
    if (type_index >= 0) ui->cb_type->setCurrentIndex(type_index);

    int cond_index = ui->cb_condition->findData(static_cast<int>(property->get_condition()));
    if (cond_index >= 0) ui->cb_condition->setCurrentIndex(cond_index);

    int energy_index = ui->cb_energy->findData(static_cast<int>(property->get_energy()));
    if (energy_index >= 0) ui->cb_energy->setCurrentIndex(energy_index);

    int country_index = ui->cb_country->findData(property->get_country());
    if (country_index >= 0) ui->cb_country->setCurrentIndex(country_index);

    ui->le_name->setText(property->get_name());
    ui->le_address->setText(property->get_address());
    ui->le_city->setText(property->get_city());
    ui->le_postal_code->setText(property->get_postal_code());
    ui->le_location_number->setText(property->get_location_number());
    ui->dsb_surface_size->setValue(property->get_surface());
  }
}

void W_Property_Creator::clear()
{
  ui->cb_type->clear();
  ui->cb_condition->clear();
  ui->cb_energy->clear();
  ui->cb_country->clear();

  ui->le_name->clear();
  ui->l_icon->clear();
  ui->le_address->clear();
  ui->le_postal_code->clear();
  ui->le_location_number->clear();
  ui->dsb_surface_size->setValue(0);
}

void W_Property_Creator::inject_data()
{
  property->set_property_type(int_to_EPropertyType(ui->cb_type->currentData().toInt()));
  property->set_condition(int_to_ECondition(ui->cb_condition->currentData().toInt()));
  property->set_energy(int_to_EEnergy(ui->cb_energy->currentData().toInt()));
  property->set_country(ui->cb_country->currentData().toString());

  property->set_name(ui->le_name->text());
  // property->set_icon(std::make_unique<Image_Buffer>(ui->l_icon->pixmap(), icon_type));
  property->set_address(ui->le_address->text());
  property->set_postal_code(ui->le_postal_code->text());
  property->set_location_number(ui->le_location_number->text());
  property->set_surface(ui->dsb_surface_size->value());
}

void W_Property_Creator::on_buttonBox_accepted()
{
  if (property->is_loaded() && property->is_dirty()) {
    property->update_record();
  } else if (property->is_dirty()) {
    property->insert_record();
  }

  if (manager) manager->refresh();
  close();
}


void W_Property_Creator::on_buttonBox_rejected()
{
  if (manager) manager->refresh();
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
