#include "widget/w_invoice.h"

#include "base.h"
#include "invoice.h"
#include "property.h"
#include "rent.h"
#include "settings.h"
#include "tenant.h"
#include "ui_w_invoice.h"
#include "widget/w_invoice_creator.h"
#include "widget/w_invoice_form.h"
#include "widget/w_invoice_manager.h"

#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QSqlQuery>


W_Invoice::W_Invoice(W_Invoice_Manager* manager, int id)
  : QWidget(manager)
  , manager(manager)
  , id(id)
  , ui(new Ui::W_Invoice)
{
  ui->setupUi(this);

  refresh();
}

void W_Invoice::refresh()
{
  Invoice invoice(id);
  Tenant  tenant(id);

  ui->l_period->setText(
      tr("From [%1] to [%2]")
          .arg(invoice.get_start_date().toString("dd/MM/yyyy"), invoice.get_end_date().toString("dd/MM/yyyy")));
  ui->l_tenant_name->setText(tenant.get_full_name());
  ui->l_rest->setText(invoice.is_rest_for_tenant() ? tr("Tenant rest") : tr("Landlord rest"));
  ui->le_rest->setText(ftom(invoice.get_rest()));
  ui->le_charges_collected->setText(ftom(invoice.get_charge_collected()));
  ui->le_other_charges->setText(ftom(invoice.get_other()));
  ui->le_last_water_check->setText(ftod(invoice.get_last_water_check()) + " m³");
  ui->le_water_check->setText(ftod(invoice.get_water_check()) + " m³");
  ui->le_water_consum->setText(ftod(invoice.get_water_consum()) + " m³");
  ui->le_water_price->setText(ftom(invoice.get_water_price()) + "/m³");
}

W_Invoice::~W_Invoice()
{
  delete ui;
}

void W_Invoice::on_b_delete_clicked()
{
  Invoice invoice(id);
  if (invoice.is_loaded()) invoice.delete_record();
}


void W_Invoice::on_b_edit_clicked()
{
  auto creator = W_Invoice_Creator(manager, id);
  creator.setModal(true);
  creator.exec();
}

void W_Invoice::on_b_print_clicked()
{
  QPrinter printer(QPrinter::HighResolution);
  printer.setOutputFormat(QPrinter::PdfFormat);
  QDir dest = PRINT_PATH();

  Invoice invoice(id);

  int     property_id = Rent::load_from_tenant(invoice.get_tenant_id(), invoice.get_start_date()).get_property_id();
  QString tenant_name = Tenant(invoice.get_tenant_id()).get_full_name();
  tenant_name.replace(" ", "_");

  QString print_path = dest.filePath(tr("Invoice_") + invoice.get_start_date().toString("yyyy_MM_dd") + "_"
                                     + Property(property_id).get_name() + "_" + tenant_name);
  printer.setOutputFileName(print_path);

  QPainter painter(&printer);

  // Nouvelle façon Qt6 pour obtenir la zone imprimable
  QRect  paintRect = printer.pageLayout().paintRectPixels(printer.resolution());
  double xscale    = double(paintRect.width()) / width();
  double yscale    = double(paintRect.height()) / height();
  double scale     = qMin(xscale * 2.5, yscale * 2.5);
  painter.scale(scale, scale);

  auto form = new W_Invoice_Form(id, rent_id_linked);
  form->render(&painter);
  painter.end();

  // Open file
  auto f = QFileInfo(print_path);
  QDesktopServices::openUrl(QUrl::fromLocalFile(f.absolutePath()));
}


void W_Invoice::on_b_send_clicked()
{
  Invoice  invoice(id);
  Tenant   tenant(invoice.get_tenant_id());
  Property property(Rent(rent_id_linked).get_property_id());

  QString dest    = tenant.get_email();
  QString subject = TXT::INVOICE_MAIL_SUBJECT.arg(invoice.get_start_date().toString("dd/MM/yyyy"),
                                                  invoice.get_end_date().toString("dd/MM/yyyy"), property.get_name());
  QString body    = TXT::INVOICE_MAIL_BODY.arg(tenant.get_full_name(), invoice.get_start_date().toString("dd/MM/yyyy"),
                                               invoice.get_end_date().toString("dd/MM/yyyy"), property.get_address(), "");

  QDesktopServices::openUrl("mailto:" + tenant.get_email() + "?subjet=" + subject + "&cc=" + "&body=" + body);
}
