#include "widget/w_rent.h"

#include "base.h"
#include "rent.h"
#include "tenant.h"
#include "ui_w_rent.h"

W_Rent::W_Rent(W_Rent_Manager* _mananger, int _id, EMonth month)
  : manager(manager)
  , id(_id)
  , ui(new Ui::W_Rent)
{
  ui->setupUi(this);

  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  ui->l_month->setText(EMonth_to_str(month));

  ui->b_new->setHidden(true);
  ui->b_delete->setHidden(true);
  ui->b_new_lease_agreement->setHidden(true);

  refresh();
}

W_Rent::~W_Rent()
{
  delete ui;
}

void W_Rent::refresh()
{
  set_block_signals(true);

  ui->cb_tenant->clear();

  ui->dsb_rent->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);
  ui->dsb_housing_aid->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);
  ui->dsb_charges->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);
  ui->dsb_house_wastes->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);

  auto q_tenants = DB_MANAGER.get_db()->all_records(ETable::tenants);
  while (q_tenants->next()) {
    Tenant tenant(q_tenants->value("tenant_id").toInt());
    ui->cb_tenant->addItem(tenant.get_full_name(), tenant.get_id());
  }

  ui->b_new->setHidden(true);

  Rent rent(id);
  if (rent.is_loaded()) {
    populate_ui();
  }

  set_block_signals(false);
}

void W_Rent::populate_ui()
{
  set_block_signals(true);

  ui->cb_tenant->clear();

  auto q_tenants = DB_MANAGER.get_db()->all_records(ETable::tenants);
  while (q_tenants->next()) {
    Tenant tenant(q_tenants->value("tenant_id").toInt());
    ui->cb_tenant->addItem(tenant.get_full_name(), tenant.get_id());
  }

  Rent rent(id);

  if (rent.is_loaded()) {
    ui->dsb_rent->setValue(rent.get_rent());
    ui->dsb_housing_aid->setValue(rent.get_housing_aid());
    ui->dsb_charges->setValue(rent.get_charge());
    ui->dsb_house_wastes->setValue(rent.get_household_waste());

    int tenant_index = ui->cb_tenant->findData(rent.get_tenant_id());
    ui->cb_tenant->setCurrentIndex(tenant_index);

    ui->te_comment->setText(rent.get_comment());
  } else {
    clear();
  }

  set_block_signals(false);
}

void W_Rent::clear()
{
  set_block_signals(true);

  ui->dsb_rent->setValue(0.0f);
  ui->dsb_housing_aid->setValue(0.0f);
  ui->dsb_charges->setValue(0.0f);
  ui->dsb_house_wastes->setValue(0.0f);

  ui->cb_tenant->currentText();

  ui->te_comment->clear();

  set_block_signals(false);
}

void W_Rent::inject_data()
{
  Rent rent(id);

  if (!rent.is_loaded()) return;

  rent.set_rent(ui->dsb_rent->value());
  rent.set_housing_aid(ui->dsb_housing_aid->value());
  rent.set_charge(ui->dsb_charges->value());
  rent.set_household_waste(ui->dsb_house_wastes->value());
  rent.set_tenant_id(ui->cb_tenant->currentData().toInt());
  rent.set_comment(ui->te_comment->document()->toMarkdown());
  rent.update_record();
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
  bool is_loaded = Rent(id).is_loaded();
  ui->b_new->setHidden(!is_loaded);
  ui->b_delete->setHidden(is_loaded);
}

void W_Rent::leaveEvent(QEvent* event)
{
  Q_UNUSED(event);
  ui->b_new->setHidden(true);
  ui->b_delete->setHidden(true);
}

void W_Rent::on_b_new_clicked()
{
  Rent rent(id);

  if (!rent.is_loaded()) {
    rent.insert_record();
  }
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
  Rent rent(id);
  if (rent.is_loaded()) rent.delete_record();
}


void W_Rent::on_b_new_lease_agreement_clicked()
{
}
