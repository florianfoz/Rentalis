#include "widget/entity/w_invoice_form.h"

#include "base.h"
#include "entity/invoice.h"
#include "entity/property.h"
#include "entity/rent.h"
#include "entity/tenant.h"
#include "ui_w_invoice_form.h"

#include <QMessageBox>

W_Invoice_Form::W_Invoice_Form(qsizetype invoice_id, qsizetype rent_id)
  : invoice(Invoice::read_record(invoice_id))
  , rent(Rent::read_record(rent_id))
  , ui(new Ui::W_Invoice_Form)
{
  ui->setupUi(this);

  auto tenant   = Tenant::read_record(invoice.tenant_id);
  auto property = Property::read_record(rent.property_id);

  if (invoice && tenant && rent && property) {
    auto consum_tot = invoice.get_water_consum_price();

    ui->l_title->setText(tr("Invoice of ") + invoice.start_date.toString("dd MMMM yyyy"));
    ui->le_tenant->setText(tenant.get_full_name());
    ui->le_period->setText(invoice.start_date.toString("dd/MM/yyyy") + tr(" to ")
                           + invoice.end_date.toString("dd/MM/yyyy"));
    ui->le_last_water_check->setText(QString::number(invoice.last_water_check) + " m³");
    ui->le_water_check->setText(QString::number(invoice.water_check) + " m³");
    ui->le_water_price->setText(ftom(invoice.water_price) + "/m³");
    ui->le_water_subscription->setText(ftom(invoice.water_subscription));
    ui->le_consum->setText(QString::number(invoice.water_check - invoice.last_water_check) + " m³");
    ui->le_consum_tot->setText(ftom(consum_tot));
    ui->le_household_waste->setText(ftom(rent.household_waste));
    ui->le_charge->setText(ftom(invoice.other));

    ui->le_charge_collected->setText(ftom(invoice.charge_collected));

    ui->le_tot->setText(ftom(consum_tot + invoice.other + invoice.water_subscription));

    ui->le_address->setText(property.address);


    float rest = consum_tot + invoice.other - invoice.charge_collected;
    if (rest <= 0) {
      ui->le_rest_info->setText(tr("Amount payable by the landlord:"));
      ui->le_rest->setText(ftom(abs(rest)));
    } else {
      ui->le_rest_info->setText(tr("Amount payable by the tenant:"));
      ui->le_rest->setText(ftom(abs(rest)));
    }
  } else {
    QString msg = QObject::tr("The invoice form printer creation for [%1] dated at [%2] have failed !")
                      .arg(tenant.get_full_name(), invoice.start_date.toString("dd MMMM yyyy"));
    QMessageBox::critical(this, tr("Invoice Form Printer Creation Failure"), msg);
  }
}

W_Invoice_Form::~W_Invoice_Form()
{
  delete ui;
}
