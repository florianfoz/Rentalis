#include "widget/w_rent.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "entities/rent.h"
#include "entities/tenant.h"
#include "ui_w_rent.h"

W_Rent::W_Rent(W_Rent_Manager* _mananger, int _id, EMonth month)
  : manager(manager)
  , id(_id)
  , ui(new Ui::W_Rent)
{
  ui->setupUi(this);

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

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

  ui->dsb_rent->setPrefix(Database_Manager::current_database()->manifest().currency.symbol);
  ui->dsb_housing_aid->setPrefix(Database_Manager::current_database()->manifest().currency.symbol);
  ui->dsb_charges->setPrefix(Database_Manager::current_database()->manifest().currency.symbol);
  ui->dsb_house_wastes->setPrefix(Database_Manager::current_database()->manifest().currency.symbol);


  if (auto q_tenants = Database_Manager::current_database()->all_records(ETable::Tenant)) {
    while (q_tenants->next()) {
      auto tenant = Tenant::read_record(q_tenants->value("tenant_id").toInt());
      ui->cb_tenant->addItem(tenant.get_full_name(), tenant.id);
    }
  }


  ui->b_new->setHidden(true);

  auto rent = Rent ::read_record(id);
  if (rent) {
    populate_ui();
  }

  set_block_signals(false);
}

void W_Rent::populate_ui()
{
  set_block_signals(true);

  ui->cb_tenant->clear();

  if (auto q_tenants = Database_Manager::current_database()->all_records(ETable::Tenant)) {
    while (q_tenants->next()) {
      auto tenant = Tenant::read_record(q_tenants->value("tenant_id").toInt());
      ui->cb_tenant->addItem(tenant.get_full_name(), tenant.id);
    }
  }

  auto rent = Rent::read_record(id);

  if (rent) {
    ui->dsb_rent->setValue(rent.rent);
    ui->dsb_housing_aid->setValue(rent.housing_aid);
    ui->dsb_charges->setValue(rent.charge);
    ui->dsb_house_wastes->setValue(rent.household_waste);

    int tenant_index = ui->cb_tenant->findData(rent.tenant_id);
    ui->cb_tenant->setCurrentIndex(tenant_index);

    ui->te_comment->setText(rent.comment);
  } else {
    clear();
  }

  set_block_signals(false);
}

void W_Rent::clear()
{
  set_block_signals(true);

  ui->dsb_rent->setValue(0.0F);
  ui->dsb_housing_aid->setValue(0.0F);
  ui->dsb_charges->setValue(0.0F);
  ui->dsb_house_wastes->setValue(0.0F);

  ui->cb_tenant->currentText();

  ui->te_comment->clear();

  set_block_signals(false);
}

void W_Rent::inject_data()
{
  auto rent = Rent::read_record(id);

  if (!rent) return;

  rent.rent            = ui->dsb_rent->value();
  rent.housing_aid     = ui->dsb_housing_aid->value();
  rent.charge          = ui->dsb_charges->value();
  rent.household_waste = ui->dsb_house_wastes->value();
  rent.tenant_id       = ui->cb_tenant->currentData().toInt();
  rent.comment         = ui->te_comment->document()->toMarkdown();
  rent.save_record();
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
  auto rent = Rent::read_record(id);
  ui->b_new->setHidden(!rent.is_valid());
  ui->b_delete->setHidden(rent.is_valid());
}

void W_Rent::leaveEvent(QEvent* event)
{
  Q_UNUSED(event);
  ui->b_new->setHidden(true);
  ui->b_delete->setHidden(true);
}

void W_Rent::on_b_new_clicked()
{
  auto rent = Rent::read_record(id);

  if (!rent) {
    Rent::create_record();
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
  auto rent = Rent::read_record(id);
  if (rent) rent.delete_record();
}


void W_Rent::on_b_new_lease_agreement_clicked()
{
}
