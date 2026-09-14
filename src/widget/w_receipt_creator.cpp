#include "widget/w_receipt_creator.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "entities/property.h"
#include "entities/receipt.h"
#include "entities/rent.h"
#include "entities/tenant.h"
#include "ui_w_receipt_creator.h"
#include "widget/w_receipt_manager.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>

W_Receipt_Creator::W_Receipt_Creator(W_Receipt_Manager* manager, int id)
  : QDialog(manager)
  , manager(manager)
  , receipt(Receipt::read_record(id))
  , ui(new Ui::W_Receipt_Creator)
{
  ui->setupUi(this);

  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");

  if (id == -1)
    setWindowTitle(tr("Receipt Creation"));
  else
    setWindowTitle(tr("Receipt Edition"));

  populate_ui();
}

W_Receipt_Creator::~W_Receipt_Creator()
{
  delete ui;
}

void W_Receipt_Creator::populate_ui()
{
  clear();

  if (auto q_tenants = Database_Manager::current_database()->all_records(ETable::Tenant)) {
    while (q_tenants->next()) {
      auto tenant = Tenant::read_record(q_tenants->value("tenant_id").toInt());
      ui->cb_tenant->addItem(tenant.get_full_name(), tenant.id);
    }
  }

  if (receipt) {
    int tenant_index = ui->cb_tenant->findData(receipt.tenant_id);
    if (tenant_index >= 0) ui->cb_tenant->setCurrentIndex(tenant_index);

    auto rent = Database_Manager::current_database()->find_property_rents_from_tenant_date(
        receipt.tenant_id, receipt.start_date, receipt.end_date);
    if (!rent->next()) {
      ui->le_charges->setText(ftom(rent->value("charge").toFloat()));
    } else {
      ui->le_charges->setText("No Charge Found!");
    }

    ui->de_date_start->setDate(receipt.start_date);
    ui->de_date_end->setDate(receipt.end_date);
    ui->de_pay_date->setDate(receipt.pay_date);

    ui->dsb_charge_advance->setValue(receipt.charge_advance);
    ui->pte_comment->setPlainText(receipt.comment);
    ui->rb_due->setChecked(receipt.due_id == EDue::Due);
    ui->rb_become_due->setChecked(!ui->rb_due->isChecked());

    update_ui();
  }
}

void W_Receipt_Creator::update_ui()
{
  inject_data();

  if (auto rent = receipt.get_rent_references()) {
    ui->le_info_tenant->setText(ui->cb_tenant->currentText());
    ui->le_info_period->setText(rent->value("date").toDate().toString("dd/MM/yyyy"));
    ui->le_info_property->setText(Property::read_record(rent->value("property_id").toInt()).name);
    ui->l_rent_issue->setText(tr("Rent found"));

    ui->le_rent->setText(ftom(rent->value("rent").toFloat() + rent->value("housing_aid").toFloat()));
    ui->le_tot->setText(ftom(receipt.get_total_receivable()));
  } else {
    ui->l_rent_issue->setText(tr("Rent not found!"));
  }
}

void W_Receipt_Creator::clear()
{
  ui->cb_tenant->setCurrentIndex(0);
  ui->de_date_start->setDate(QDate::currentDate());
  ui->de_date_end->setDate(QDate::currentDate().addMonths(1));
  ui->de_pay_date->setDate(QDate::currentDate().addMonths(1));
  ui->dsb_charge_advance->setValue(0.0F);
  ui->dsb_charge_advance->setPrefix(Database_Manager::current_database()->manifest().currency.symbol);
  ui->pte_comment->setPlainText("");
  ui->rb_due->setChecked(true);
  ui->rb_become_due->setChecked(false);
  ui->le_info_tenant->setText("");
  ui->le_info_period->setText("");
  ui->le_info_property->setText("");
  ui->l_rent_issue->setText(tr("Rent not found!"));
}

void W_Receipt_Creator::inject_data()
{
  int tenant_id          = ui->cb_tenant->currentData().toInt();
  receipt.tenant_id      = ui->cb_tenant->currentData().toInt();
  receipt.start_date     = ui->de_date_start->date();
  receipt.end_date       = ui->de_date_end->date();
  receipt.pay_date       = ui->de_pay_date->date();
  receipt.charge_advance = ui->dsb_charge_advance->value();
  receipt.comment        = ui->pte_comment->toPlainText();
  receipt.due_id         = ui->rb_due->isChecked() ? EDue::Due : EDue::WillDue;
}

void W_Receipt_Creator::on_buttonBox_accepted()
{
  (void)receipt.save_record();

  if (manager) manager->refresh();
  close();
}


void W_Receipt_Creator::on_buttonBox_rejected()
{
  if (manager) manager->refresh();
  close();
}

void W_Receipt_Creator::on_cb_tenant_currentIndexChanged(int index)
{
  update_ui();
}


void W_Receipt_Creator::on_de_date_start_userDateChanged(const QDate& date)
{
  update_ui();
}


void W_Receipt_Creator::on_de_date_end_userDateChanged(const QDate& date)
{
  update_ui();
}
