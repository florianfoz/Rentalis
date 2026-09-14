#include "widget/w_invoice.h"

#include "base.h"
#include "entities/invoice.h"
#include "entities/property.h"
#include "entities/rent.h"
#include "entities/tenant.h"
#include "rentalis_settings.h"
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
  auto invoice = Invoice::read_record(id);
  auto tenant  = Tenant::read_record(id);

  ui->l_period->setText(
      tr("From [%1] to [%2]").arg(invoice.start_date.toString("dd/MM/yyyy"), invoice.end_date.toString("dd/MM/yyyy")));
  ui->l_tenant_name->setText(tenant.get_full_name());
  ui->l_rest->setText(invoice.is_rest_for_tenant() ? tr("Tenant rest") : tr("Landlord rest"));
  ui->le_rest->setText(ftom(invoice.get_rest()));
  ui->le_charges_collected->setText(ftom(invoice.charge_collected));
  ui->le_other_charges->setText(ftom(invoice.other));
  ui->le_last_water_check->setText(ftod(invoice.last_water_check) + " m³");
  ui->le_water_check->setText(ftod(invoice.water_check) + " m³");
  ui->le_water_consum->setText(ftod(invoice.get_water_consum()) + " m³");
  ui->le_water_price->setText(ftom(invoice.water_price) + "/m³");
}

W_Invoice::~W_Invoice()
{
  delete ui;
}

void W_Invoice::on_b_delete_clicked()
{
  auto invoice = Invoice::read_record(id);
  if (invoice) invoice.delete_record();
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

  auto invoice = Invoice::read_record(id);

  int     property_id = Rent::load_from_tenant(invoice.tenant_id, invoice.start_date).property_id;
  QString tenant_name = Tenant::read_record(invoice.tenant_id).get_full_name();
  tenant_name.replace(" ", "_");

  QString print_path = dest.filePath(tr("Invoice_") + invoice.start_date.toString("yyyy_MM_dd") + "_"
                                     + Property::read_record(property_id).name + "_" + tenant_name);
  printer.setOutputFileName(print_path);

  QPainter painter(&printer);

  // Nouvelle façon Qt6 pour obtenir la zone imprimable
  QRect  paintRect = printer.pageLayout().paintRectPixels(printer.resolution());
  double xscale    = double(paintRect.width()) / width();
  double yscale    = double(paintRect.height()) / height();
  double scale     = qMin(xscale * 2.5, yscale * 2.5);
  painter.scale(scale, scale);

  auto* form = new W_Invoice_Form(id, rent_id_linked);
  form->render(&painter);
  painter.end();

  // Open file
  auto f = QFileInfo(print_path);
  QDesktopServices::openUrl(QUrl::fromLocalFile(f.absolutePath()));
}


void W_Invoice::on_b_send_clicked()
{
  auto invoice  = Invoice::read_record(id);
  auto tenant   = Tenant::read_record(invoice.tenant_id);
  auto property = Property::read_record(Rent::read_record(rent_id_linked).property_id);

  QString dest    = tenant.email;
  QString subject = TXT::INVOICE_MAIL_SUBJECT.arg(invoice.start_date.toString("dd/MM/yyyy"),
                                                  invoice.end_date.toString("dd/MM/yyyy"), property.name);
  QString body    = TXT::INVOICE_MAIL_BODY.arg(tenant.get_full_name(), invoice.start_date.toString("dd/MM/yyyy"),
                                               invoice.end_date.toString("dd/MM/yyyy"), property.address, "");

  QDesktopServices::openUrl("mailto:" + tenant.email + "?subjet=" + subject + "&cc=" + "&body=" + body);
}
