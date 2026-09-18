#include "widget/entity/w_rent.h"

#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "entity/property.h"
#include "entity/rent.h"
#include "entity/tenant.h"
#include "preferences.h"
#include "ui_w_rent.h"

W_Rent::W_Rent(QDate date, qsizetype _property_id)
  : ui(new Ui::W_Rent)
{
  ui->setupUi(this);

  rent.date        = date;
  rent.property_id = _property_id;

  ui->l_month->setText(Preferences::locale().toString(rent.date, "MMMM"));

  auto& sym = Database_Manager::current_manifest()->currency.symbol;

  ui->dsb_rent->setPrefix(sym);
  ui->dsb_housing_aid->setPrefix(sym);
  ui->dsb_charges->setPrefix(sym);
  ui->dsb_house_wastes->setPrefix(sym);

  ui->b_new_lease_agreement->setHidden(true);

  not_created = true;

  refresh();
}

W_Rent::W_Rent(qsizetype _id)
  : rent(Rent::read_record(_id))
  , ui(new Ui::W_Rent)
{
  assert(rent && "Invalid rent, use W_Rent(QDate, qsizetype) instead");

  ui->setupUi(this);

  ui->l_month->setText(Preferences::locale().toString(rent.date, "MMMM"));

  auto& sym = Database_Manager::current_manifest()->currency.symbol;

  ui->dsb_rent->setPrefix(sym);
  ui->dsb_housing_aid->setPrefix(sym);
  ui->dsb_charges->setPrefix(sym);
  ui->dsb_house_wastes->setPrefix(sym);

  ui->b_new_lease_agreement->setHidden(true);

  refresh();
}

W_Rent::~W_Rent()
{
  delete ui;
}

void W_Rent::refresh()
{
  if (rent.property_id == INVALID_ID)
    ui->stackedWidget->setCurrentIndex(0);
  else
    ui->stackedWidget->setCurrentIndex(2);

  if (!rent.is_valid()) {
    dosent_exist();
    return;
  }

  set_block_signals(true);

  ui->cb_tenant->clear();

  for (const auto& rec : Tenant::all_records()) {
    ui->cb_tenant->addItem(rec.get_full_name(), rec.id);
  }


  ui->dsb_rent->setValue(rent.rent);
  ui->dsb_housing_aid->setValue(rent.housing_aid);
  ui->dsb_charges->setValue(rent.charge);
  ui->dsb_house_wastes->setValue(rent.household_waste);

  int tenant_index = ui->cb_tenant->findData(rent.tenant_id);
  ui->cb_tenant->setCurrentIndex(tenant_index);

  ui->te_comment->setText(rent.comment);
  ui->b_delete->show();

  set_block_signals(false);
}

void W_Rent::dosent_exist()
{
  set_block_signals(true);

  ui->cb_tenant->clear();

  ui->dsb_rent->setValue(0.0F);
  ui->dsb_housing_aid->setValue(0.0F);
  ui->dsb_charges->setValue(0.0F);
  ui->dsb_house_wastes->setValue(0.0F);

  ui->cb_tenant->currentText();

  ui->te_comment->clear();

  ui->b_delete->hide();

  set_block_signals(false);
}

void W_Rent::inject_data()
{
  rent.rent            = (float)ui->dsb_rent->value();
  rent.housing_aid     = (float)ui->dsb_housing_aid->value();
  rent.charge          = (float)ui->dsb_charges->value();
  rent.household_waste = (float)ui->dsb_house_wastes->value();
  rent.tenant_id       = ui->cb_tenant->currentData().toInt();
  rent.comment         = ui->te_comment->document()->toMarkdown();
}

void W_Rent::set_block_signals(bool block)
{
  ui->dsb_rent->blockSignals(block);
  ui->dsb_charges->blockSignals(block);
  ui->dsb_housing_aid->blockSignals(block);
  ui->dsb_house_wastes->blockSignals(block);
  ui->te_comment->blockSignals(block);
  ui->cb_tenant->blockSignals(block);
}


void W_Rent::enterEvent(QEnterEvent* event)
{
  Q_UNUSED(event);

  if (rent.property_id == INVALID_ID)
    ui->stackedWidget->setCurrentIndex(0);
  else
    ui->stackedWidget->setCurrentIndex(rent.is_valid() ? 2 : 1);
}

void W_Rent::leaveEvent(QEvent* event)
{
  Q_UNUSED(event);

  if (rent.property_id == INVALID_ID)
    ui->stackedWidget->setCurrentIndex(0);
  else
    ui->stackedWidget->setCurrentIndex(2);

  if (!not_created && rent.is_dirty()) (void)rent.save_record();
}

void W_Rent::on_b_new_clicked()
{
  (void)rent.save_record();
}


void W_Rent::on_dsb_rent_valueChanged(double arg1)
{
  inject_data();
}


void W_Rent::on_dsb_housing_aid_textChanged(const QString& arg1)
{
  inject_data();
}


void W_Rent::on_dsb_charges_textChanged(const QString& arg1)
{
  inject_data();
}


void W_Rent::on_dsb_house_wastes_textChanged(const QString& arg1)
{
  inject_data();
}


void W_Rent::on_cb_tenant_currentIndexChanged(int index)
{
  inject_data();
}


void W_Rent::on_te_comment_textChanged()
{
  inject_data();
}


void W_Rent::on_b_delete_clicked()
{
  auto property = Property::read_record(rent.property_id);

  (void)rent.delete_record(
      true, tr(" from %1 for property %2").arg(Preferences::locale().toString(rent.date, "MMMM yyyy"), property.name));
}


void W_Rent::on_b_new_lease_agreement_clicked()
{
}
