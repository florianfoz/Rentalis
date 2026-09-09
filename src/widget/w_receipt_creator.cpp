#include "widget/w_receipt_creator.h"

#include "base.h"
#include "property.h"
#include "receipt.h"
#include "rent.h"
#include "tenant.h"
#include "ui_w_receipt_creator.h"
#include "widget/w_receipt_manager.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>

W_Receipt_Creator::W_Receipt_Creator(W_Receipt_Manager* manager, int id)
  : QDialog(manager)
  , manager(manager)
  , receipt(new Receipt(id))
  , ui(new Ui::W_Receipt_Creator)
{
  ui->setupUi(this);

  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Apply))
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

  if (auto q_tenants = DB_MANAGER.get_db()->all_records(ETable::tenants)) {
    while (q_tenants->next()) {
      Tenant tenant(q_tenants->value("tenant_id").toInt());
      ui->cb_tenant->addItem(tenant.get_full_name(), tenant.get_id());
    }
  }

  if (receipt->is_loaded()) {
    int tenant_index = ui->cb_tenant->findData(static_cast<int>(receipt->get_tenant_id()));
    if (tenant_index >= 0) ui->cb_tenant->setCurrentIndex(tenant_index);

    auto rent = DB_MANAGER.get_db()->find_property_rents_from_tenant_date(
        receipt->get_tenant_id(), receipt->get_start_date(), receipt->get_end_date());
    if (!rent->next()) {
      ui->le_charges->setText(ftom(rent->value("charge").toFloat()));
    } else {
      ui->le_charges->setText("No Charge Found!");
    }

    ui->de_date_start->setDate(receipt->get_start_date());
    ui->de_date_end->setDate(receipt->get_end_date());
    ui->de_pay_date->setDate(receipt->get_pay_date());

    ui->dsb_charge_advance->setValue(receipt->get_charge_advance());
    ui->pte_comment->setPlainText(receipt->get_comment());
    ui->rb_due->setChecked(receipt->get_due() == EDue::Due);
    ui->rb_become_due->setChecked(!ui->rb_due->isChecked());

    update_ui();
  }
}

void W_Receipt_Creator::update_ui()
{
  inject_data();

  if (auto rent = receipt->get_rent_references()) {
    ui->le_info_tenant->setText(ui->cb_tenant->currentText());
    ui->le_info_period->setText(rent->value("date").toDate().toString("dd/MM/yyyy"));
    ui->le_info_property->setText(Property(rent->value("property_id").toInt()).get_name());
    ui->l_rent_issue->setText(tr("Rent found"));

    ui->le_rent->setText(ftom(rent->value("rent").toFloat() + rent->value("housing_aid").toFloat()));
    ui->le_tot->setText(ftom(receipt->get_total_receivable()));
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
  ui->dsb_charge_advance->setValue(0.0f);
  ui->dsb_charge_advance->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);
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
  int tenant_id = ui->cb_tenant->currentData().toInt();
  receipt->set_tenant_id(ui->cb_tenant->currentData().toInt());
  receipt->set_start_date(ui->de_date_start->date());
  receipt->set_end_date(ui->de_date_end->date());
  receipt->set_pay_date(ui->de_pay_date->date());
  receipt->set_charge_advance(ui->dsb_charge_advance->value());
  receipt->set_comment(ui->pte_comment->toPlainText());
  receipt->set_due(ui->rb_due->isChecked() ? EDue::Due : EDue::WillDue);
}

void W_Receipt_Creator::on_buttonBox_accepted()
{
  if (receipt->is_loaded() && receipt->is_dirty()) {
    receipt->update_record();
  } else if (receipt->is_dirty()) {
    receipt->insert_record();
  }

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
