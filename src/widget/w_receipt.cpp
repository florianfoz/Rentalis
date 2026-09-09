#include "widget/w_receipt.h"

#include "base.h"
#include "receipt.h"
#include "tenant.h"
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
  auto receipt = Receipt(id);
  auto rent    = DB_MANAGER.get_db()->find_property_rents_from_tenant_date(
      receipt.get_tenant_id(), receipt.get_start_date(), receipt.get_end_date());
  ui->l_period->setText(
      tr("From %1 to %2")
          .arg(receipt.get_start_date().toString("dd/MM/yyyy"), receipt.get_end_date().toString("dd/MM/yyyy")));
  ui->l_tenant->setText(Tenant(receipt.get_tenant_id()).get_full_name());
  ui->le_paydate->setText(receipt.get_pay_date().toString("dd/MM/yyyy"));
  ui->le_charges_advance->setText(ftom(receipt.get_charge_advance()));
  ui->te_comment->setText(receipt.get_comment());
  ui->le_status->setText(EDue_to_str(receipt.get_due()));

  if (rent->exec() && rent->next()) {
    float rent_val = rent->value("rent").toFloat();
    float charges  = rent->value("charges").toFloat();
    ui->le_rent->setText(ftom(rent_val));
    float tot = rent_val + charges - receipt.get_charge_advance();
    ui->le_tot->setText(ftom(tot));
  } else {
    qWarning() << "Rent for tenant: " << receipt.get_tenant_id() << " at date "
               << receipt.get_start_date().toString("dd/MM/yyyy") << " not found";
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
  Receipt receipt(id);

  QString msg = QObject::tr("Do you really want to delete the receipt for tenant [%1] dated from [%2 to %3]?\n")
                    .arg(Tenant(receipt.get_tenant_id()).get_full_name())
                    .arg(receipt.get_start_date().toString("dd/MM/yyyy"))
                    .arg(receipt.get_end_date().toString("dd/MM/yyyy"));

  auto result = QMessageBox::warning(this, QObject::tr("Delete Receipt"), msg + TXT::WARNING_OPERATION,
                                     QMessageBox::Yes | QMessageBox::Cancel);

  if (result == QMessageBox::Cancel) return;

  receipt.delete_record();
}
