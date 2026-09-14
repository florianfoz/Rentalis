#include "widget/w_receipt.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "entities/receipt.h"
#include "entities/tenant.h"
#include "ui_w_receipt.h"
#include "widget/w_receipt_creator.h"
#include "widget/w_receipt_manager.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QSqlQuery>

W_Receipt::W_Receipt(W_Receipt_Manager* manager, int id)
  : id(id)
  , manager(manager)
  , ui(new Ui::W_Receipt)
{
  ui->setupUi(this);

  refresh();
}

void W_Receipt::refresh()
{
  auto receipt = Receipt::read_record(id);
  auto rent    = Database_Manager::current_database()->find_property_rents_from_tenant_date(
      receipt.tenant_id, receipt.start_date, receipt.end_date);
  ui->l_period->setText(
      tr("From %1 to %2").arg(receipt.start_date.toString("dd/MM/yyyy"), receipt.end_date.toString("dd/MM/yyyy")));
  ui->l_tenant->setText(Tenant::read_record(receipt.tenant_id).get_full_name());
  ui->le_paydate->setText(receipt.pay_date.toString("dd/MM/yyyy"));
  ui->le_charges_advance->setText(ftom(receipt.charge_advance));
  ui->te_comment->setText(receipt.comment);
  ui->le_status->setText(EDue_to_str(receipt.due_id));

  if (rent->exec() && rent->next()) {
    float rent_val = rent->value("rent").toFloat();
    float charges  = rent->value("charges").toFloat();
    ui->le_rent->setText(ftom(rent_val));
    float tot = rent_val + charges - receipt.charge_advance;
    ui->le_tot->setText(ftom(tot));
  } else {
    qWarning() << "Rent for tenant: " << receipt.tenant_id << " at date " << receipt.start_date.toString("dd/MM/yyyy")
               << " not found";
  }
}

W_Receipt::~W_Receipt()
{
  delete ui;
}

void W_Receipt::on_b_edit_clicked()
{
  auto receipt = W_Receipt_Creator(manager, id);
  receipt.setModal(true);
  receipt.exec();
}


void W_Receipt::on_b_delete_clicked()
{
  auto receipt = Receipt::read_record(id);

  QString msg = QObject::tr("Do you really want to delete the receipt for tenant [%1] dated from [%2 to %3]?\n")
                    .arg(Tenant::read_record(receipt.tenant_id).get_full_name())
                    .arg(receipt.start_date.toString("dd/MM/yyyy"))
                    .arg(receipt.end_date.toString("dd/MM/yyyy"));

  auto result = QMessageBox::warning(this, QObject::tr("Delete Receipt"), msg + TXT::WARNING_OPERATION,
                                     QMessageBox::Yes | QMessageBox::Cancel);

  if (result == QMessageBox::Cancel) return;

  receipt.delete_record();
}
