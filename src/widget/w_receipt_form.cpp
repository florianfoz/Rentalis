#include "widget/w_receipt_form.h"

#include "base.h"
#include "entities/property.h"
#include "entities/receipt.h"
#include "entities/rent.h"
#include "entities/tenant.h"
#include "ui_w_receipt_form.h"

#include <QMessageBox>

W_Receipt_Form::W_Receipt_Form(int receipt_id, int rent_id)
  : receipt_id(receipt_id)
  , rent_id(rent_id)
  , ui(new Ui::W_Receipt_Form)
{
  ui->setupUi(this);

  auto receipt  = Receipt::read_record(receipt_id);
  auto tenant   = Tenant::read_record(receipt.tenant_id);
  auto rent     = Rent::read_record(rent_id);
  auto property = Property::read_record(rent.property_id);

  if (receipt && tenant && rent && property) {
    float tot = receipt.get_total_receivable();

    ui->le_rent->setText(ftom(rent.rent));
    ui->l_title->setText(tr("Location receipt of ") + receipt.start_date.toString("dd MMMM yyyy"));
    ui->le_date_period->setText(receipt.start_date.toString("dd/MM/yyyy") + tr(" to ")
                                + receipt.end_date.toString("dd/MM/yyyy"));
    ui->le_start_date->setText(receipt.start_date.toString("dd/MM/yyyy"));
    ui->le_charge_advance->setText(ftom(receipt.charge_advance));
    ui->le_tenant_name->setText(tenant.get_full_name());
    ui->le_tot->setText(ftom(tot));
    ui->le_tot_amount->setText(ui->le_tot->displayText());
    ui->le_property_address->setText(property.address);
    ui->rb_due->setChecked(receipt.due_id == EDue::Due);
    ui->re_will_due->setChecked(!ui->rb_due->isChecked());
    ui->le_pay_date->setText(receipt.pay_date.toString("dd/MM/yyyy"));
  } else {
    QString msg = QObject::tr("The receipt form printer creation for [%1] dated at [%2] have failed !")
                      .arg(tenant.get_full_name(), receipt.start_date.toString("dd MMMM yyyy"));
    QMessageBox::critical(this, tr("Receipt Form Printer Creation Failure"), msg);
  }
}

W_Receipt_Form::~W_Receipt_Form()
{
  delete ui;
}
