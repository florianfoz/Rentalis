#include "widget/entity/w_receipt_creator.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "entity/property.h"
#include "entity/receipt.h"
#include "entity/rent.h"
#include "entity/tenant.h"
#include "ui_w_receipt_creator.h"
#include "widget/entity/w_receipt_manager.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>

W_Receipt_Creator::W_Receipt_Creator(qsizetype id)
  : QDialog(nullptr)
  , receipt(Receipt::read_record(id))
  , ui(new Ui::W_Receipt_Creator)
{
  ui->setupUi(this);

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

  for (const auto& rec : Tenant::all_records()) {
    ui->cb_tenant->addItem(rec.get_full_name(), rec.id);
  }

  if (receipt) {
    int tenant_index = ui->cb_tenant->findData(receipt.tenant_id);
    if (tenant_index >= 0) ui->cb_tenant->setCurrentIndex(tenant_index);

    auto rent = Rent::find_by_tenant_date(receipt.tenant_id, receipt.start_date, receipt.end_date);
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
    ui->rb_due->setChecked(receipt.due == EDue::Due);
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
  ui->dsb_charge_advance->setPrefix(Database_Manager::current_manifest()->currency.symbol);
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
  receipt.tenant_id      = ui->cb_tenant->currentData().toInt();
  receipt.start_date     = ui->de_date_start->date();
  receipt.end_date       = ui->de_date_end->date();
  receipt.pay_date       = ui->de_pay_date->date();
  receipt.charge_advance = ui->dsb_charge_advance->value();
  receipt.comment        = ui->pte_comment->toPlainText();
  receipt.due            = ui->rb_due->isChecked() ? EDue::Due : EDue::WillDue;
}

void W_Receipt_Creator::on_buttonBox_accepted()
{
  inject_data();
  (void)receipt.save_record();
  close();
}


void W_Receipt_Creator::on_buttonBox_rejected()
{
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
