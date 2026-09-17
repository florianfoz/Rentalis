#include "widget/entity/w_invoice_creator.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "entity/invoice.h"
#include "entity/tenant.h"
#include "preferences.h"
#include "ui_w_invoice_creator.h"
#include "widget/entity/w_invoice_manager.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>

W_Invoice_Creator::W_Invoice_Creator(qsizetype id)
  : QDialog(nullptr)
  , invoice(Invoice::read_record(id))
  , ui(new Ui::W_Invoice_Creator)
{
  ui->setupUi(this);

  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");

  if (invoice)
    setWindowTitle(tr("Invoice Edition"));
  else
    setWindowTitle(tr("Invoice Creation"));

  populate_ui();
}

W_Invoice_Creator::~W_Invoice_Creator()
{
  delete ui;
}

void W_Invoice_Creator::populate_ui()
{
  clear();

  if (invoice) {
    int tenant_index = ui->cb_tenant->findData(invoice.tenant_id);
    if (tenant_index >= 0) ui->cb_tenant->setCurrentIndex(tenant_index);

    ui->dsb_charge_collected->setValue(invoice.charge_collected);
    ui->dsb_other->setValue(invoice.other);
    ui->de_date_start->setDate(invoice.start_date);
    ui->de_date_end->setDate(invoice.end_date);

    ui->dsb_water_price->setValue(invoice.water_price);
    ui->dsb_water_subscription->setValue(invoice.water_subscription);
    ui->sb_water_check->setValue(invoice.water_check);
    ui->sb_last_water_check->setValue(invoice.last_water_check);
    ui->le_water_consum->setText(ftod(invoice.get_water_consum()));
    ui->le_water_consum_price->setText(ftom(invoice.get_water_consum_price()));

    ui->te_comment->setText(invoice.comment);

    ui->le_rest->setText(ftom(invoice.get_rest()));
    ui->l_rest->setText(invoice.is_rest_for_tenant() ? tr("Tenant rest") : tr("Landlord rest"));
  }
}

void W_Invoice_Creator::clear()
{
  ui->cb_tenant->clear();
  for (const auto& rec : Tenant::all_records()) {
    ui->cb_tenant->addItem(rec.get_full_name(), rec.id);
  }

  ui->de_date_start->setDate(QDate::currentDate());
  ui->de_date_end->setDate(QDate::currentDate().addMonths(1));

  ui->dsb_charge_collected->setValue(0.0F);
  ui->dsb_charge_collected->setPrefix(Database_Manager::current_manifest()->currency.symbol);
  ui->dsb_other->setValue(0.0F);
  ui->dsb_other->setPrefix(Database_Manager::current_manifest()->currency.symbol);
  ui->dsb_water_price->setValue(1.0F);
  ui->dsb_water_price->setPrefix(Database_Manager::current_manifest()->currency.symbol);
  ui->dsb_water_subscription->setValue(0.0F);
  ui->dsb_water_subscription->setPrefix(Database_Manager::current_manifest()->currency.symbol);
  ui->sb_last_water_check->setValue(0);
  ui->sb_water_check->setValue(0);

  ui->te_comment->clear();

  ui->le_rest->setText(ftom(0.0F));
  ui->l_rest->setText("rest unknown");
}

void W_Invoice_Creator::inject_data()
{
  invoice.tenant_id = ui->cb_tenant->currentData().toInt();

  invoice.charge_collected = ui->dsb_charge_collected->value();
  invoice.other            = ui->dsb_other->value();
  invoice.start_date       = ui->de_date_start->date();
  invoice.end_date         = ui->de_date_end->date();

  invoice.water_price        = ui->dsb_water_price->value();
  invoice.water_subscription = ui->dsb_water_subscription->value();
  invoice.water_check        = ui->sb_water_check->value();
  invoice.last_water_check   = ui->sb_last_water_check->value();

  invoice.comment = ui->te_comment->document()->toMarkdown();
}

void W_Invoice_Creator::check_data()
{
  if (!Preferences::invoice_warning("invoice").toBool()) return;

  if (Preferences::invoice_warning("water_price").toBool()
      && ui->dsb_water_price->value() <= Preferences::invoice_warning("water_price_threshold").toFloat()) {
    QMessageBox::warning(this, tr("Water price low"),
                         tr("The water price is particulary low (%1 m³), it is normal ?\n"
                            "You can disable the water price warning in parameters.")
                             .arg(ftod(ui->dsb_water_price->value())));
    return;
  }

  if (Preferences::invoice_warning("water_subscription").toBool()
      && ui->dsb_water_subscription->value()
             <= Preferences::invoice_warning("water_subscription_threshold").toFloat()) {
    QMessageBox::warning(this, tr("Water subscription low"),
                         tr("The water subscription is particulary low (%1 m³), it is normal ?\n"
                            "You can disable the water subscription warning in parameters.")
                             .arg(ftod(ui->dsb_water_subscription->value())));
    return;
  }

  if (Preferences::invoice_warning("water_last_check").toBool()
      && ui->sb_last_water_check->value() <= Preferences::invoice_warning("water_last_check_threshold").toFloat()) {
    QMessageBox::warning(this, tr("Water last water check low"),
                         tr("The last water check is particulary low (%1 m³), it is normal ?\n"
                            "You can disable the last water check warning in parameters.")
                             .arg(ftod(ui->sb_last_water_check->value())));
    return;
  }

  if (Preferences::invoice_warning("water_check").toBool()
      && ui->sb_water_check->value() <= Preferences::invoice_warning("water_check_threshold").toFloat()) {
    QMessageBox::warning(this, tr("Water check low"),
                         tr("The water check is particulary low (%1 m³), it is normal ?\n"
                            "You can disable the water check warning in parameters.")
                             .arg(ftod(ui->sb_water_check->value())));
    return;
  }

  if (Preferences::general_date && Preferences::invoice_warning("dates").toBool()
      && ui->de_date_start->date().toJulianDay() > ui->de_date_end->date().toJulianDay()) {
    QMessageBox::warning(
        this, tr("Date conflit"),
        tr("The start date (%1) is posterior at the end date (%2), it is normal ?\n"
           "You can disable the date warning in parameters.")
            .arg(ui->de_date_start->date().toString("dd/MM/yyyy"), ui->de_date_end->date().toString("dd/MM/yyyy")));
    return;
  }
}

void W_Invoice_Creator::on_buttonBox_accepted()
{
  inject_data();
  (void)invoice.save_record();
  close();
}


void W_Invoice_Creator::on_buttonBox_rejected()
{
  close();
}
