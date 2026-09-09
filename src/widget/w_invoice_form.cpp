#include "widget/w_invoice_form.h"

#include "base.h"
#include "invoice.h"
#include "property.h"
#include "rent.h"
#include "tenant.h"
#include "ui_w_invoice_form.h"

#include <QMessageBox>

W_Invoice_Form::W_Invoice_Form(int invoice_id, int rent_id)
  : invoice_id(invoice_id)
  , rent_id(rent_id)
  , ui(new Ui::W_Invoice_Form)
{
  ui->setupUi(this);

  auto invoice  = Invoice(invoice_id);
  auto tenant   = Tenant(invoice.get_tenant_id());
  auto rent     = Rent(rent_id);
  auto property = Property(rent.get_property_id());

  if (invoice.is_loaded() && tenant.is_loaded() && rent.is_loaded() && property.is_loaded()) {
    auto consum_tot = (invoice.get_water_check() - invoice.get_last_water_check()) * invoice.get_water_price();

    ui->l_title->setText(tr("Invoice of ") + invoice.get_start_date().toString("dd MMMM yyyy"));
    ui->le_tenant->setText(tenant.get_full_name());
    ui->le_period->setText(invoice.get_start_date().toString("dd/MM/yyyy") + tr(" to ")
                           + invoice.get_end_date().toString("dd/MM/yyyy"));
    ui->le_last_water_check->setText(QString::number(invoice.get_last_water_check()) + " m³");
    ui->le_water_check->setText(QString::number(invoice.get_water_check()) + " m³");
    ui->le_water_price->setText(ftom(invoice.get_water_price()) + "/m³");
    ui->le_water_subscription->setText(ftom(invoice.get_water_subscription()));
    ui->le_consum->setText(QString::number(invoice.get_water_check() - invoice.get_last_water_check()) + " m³");
    ui->le_consum_tot->setText(ftom(consum_tot));
    ui->le_household_waste->setText(ftom(rent.get_household_waste()));
    ui->le_charge->setText(ftom(invoice.get_other()));

    ui->le_charge_collected->setText(ftom(invoice.get_charge_collected()));

    ui->le_tot->setText(ftom(consum_tot + invoice.get_other() + invoice.get_water_subscription()));

    ui->le_address->setText(property.get_address());


    float rest = consum_tot + invoice.get_other() - invoice.get_charge_collected();
    if (rest <= 0) {
      ui->le_rest_info->setText(tr("Amount payable by the landlord:"));
      ui->le_rest->setText(ftom(abs(rest)));
    } else {
      ui->le_rest_info->setText(tr("Amount payable by the tenant:"));
      ui->le_rest->setText(ftom(abs(rest)));
    }
  } else {
    QString msg = QObject::tr("The invoice form printer creation for [%1] dated at [%2] have failed !")
                      .arg(tenant.get_full_name(), invoice.get_start_date().toString("dd MMMM yyyy"));
    QMessageBox::critical(this, tr("Invoice Form Printer Creation Failure"), msg);
  }
}

W_Invoice_Form::~W_Invoice_Form()
{
  delete ui;
}
