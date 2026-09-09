#include "widget/w_invoice_creator.h"

#include "base.h"
#include "invoice.h"
#include "settings.h"
#include "tenant.h"
#include "ui_w_invoice_creator.h"
#include "widget/w_invoice_manager.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>

W_Invoice_Creator::W_Invoice_Creator(W_Invoice_Manager* manager, int id)
  : QDialog(manager)
  , manager(manager)
  , invoice(new Invoice(id))
  , ui(new Ui::W_Invoice_Creator)
{
  ui->setupUi(this);

  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");

  if (id == -1)
    setWindowTitle(tr("Invoice Creation"));
  else
    setWindowTitle(tr("Invoice Edition"));

  populate_ui();
}

W_Invoice_Creator::~W_Invoice_Creator()
{
  delete ui;
}

void W_Invoice_Creator::populate_ui()
{
  clear();

  if (invoice->is_loaded()) {
    int tenant_index = ui->cb_tenant->findData(invoice->get_tenant_id());
    if (tenant_index >= 0) ui->cb_tenant->setCurrentIndex(tenant_index);

    ui->dsb_charge_collected->setValue(invoice->get_charge_collected());
    ui->dsb_other->setValue(invoice->get_other());
    ui->de_date_start->setDate(invoice->get_start_date());
    ui->de_date_end->setDate(invoice->get_end_date());

    ui->dsb_water_price->setValue(invoice->get_water_price());
    ui->dsb_water_subscription->setValue(invoice->get_water_subscription());
    ui->sb_water_check->setValue(invoice->get_water_check());
    ui->sb_last_water_check->setValue(invoice->get_last_water_check());
    ui->le_water_consum->setText(ftod(invoice->get_water_consum()));
    ui->le_water_consum_price->setText(ftom(invoice->get_water_consum_price()));

    ui->te_comment->setText(invoice->get_comment());

    ui->le_rest->setText(ftom(invoice->get_rest()));
    ui->l_rest->setText(invoice->is_rest_for_tenant() ? tr("Tenant rest") : tr("Landlord rest"));
  }
}

void W_Invoice_Creator::clear()
{
  ui->cb_tenant->clear();
  if (auto q_tenants = DB_MANAGER.get_db()->all_records(ETable::tenants)) {
    while (q_tenants->next()) {
      Tenant tenant(q_tenants->value("tenant_id").toInt());
      ui->cb_tenant->addItem(tenant.get_full_name(), tenant.get_id());
    }
  }

  ui->de_date_start->setDate(QDate::currentDate());
  ui->de_date_end->setDate(QDate::currentDate().addMonths(1));

  ui->dsb_charge_collected->setValue(0.0f);
  ui->dsb_charge_collected->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);
  ui->dsb_other->setValue(0.0f);
  ui->dsb_other->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);
  ui->dsb_water_price->setValue(1.0f);
  ui->dsb_water_price->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);
  ui->dsb_water_subscription->setValue(0.0f);
  ui->dsb_water_subscription->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);
  ui->sb_last_water_check->setValue(0);
  ui->sb_water_check->setValue(0);

  ui->te_comment->clear();

  ui->le_rest->setText(ftom(0.0f));
  ui->l_rest->setText("rest unknown");
}

void W_Invoice_Creator::inject_data()
{
  invoice->set_tenant_id(ui->cb_tenant->currentData().toInt());

  invoice->set_charge_collected(ui->dsb_charge_collected->value());
  invoice->set_other(ui->dsb_other->value());
  invoice->set_start_date(ui->de_date_start->date());
  invoice->set_end_date(ui->de_date_end->date());

  invoice->set_water_price(ui->dsb_water_price->value());
  invoice->set_water_subscription(ui->dsb_water_subscription->value());
  invoice->set_water_check(ui->sb_water_check->value());
  invoice->set_last_water_check(ui->sb_last_water_check->value());

  invoice->set_comment(ui->te_comment->document()->toMarkdown());
}

void W_Invoice_Creator::check_data()
{
  if (!SETTINGS.get_invoice_warning("invoice").toBool()) return;

  if (SETTINGS.get_invoice_warning("water_price").toBool()
      && ui->dsb_water_price->value() <= SETTINGS.get_invoice_warning("water_price_threshold").toFloat()) {
    QMessageBox::warning(this, tr("Water price low"),
                         tr("The water price is particulary low (%1 m³), it is normal ?\n"
                            "You can disable the water price warning in parameters.")
                             .arg(ftod(ui->dsb_water_price->value())));
    return;
  }

  if (SETTINGS.get_invoice_warning("water_subscription").toBool()
      && ui->dsb_water_subscription->value()
             <= SETTINGS.get_invoice_warning("water_subscription_threshold").toFloat()) {
    QMessageBox::warning(this, tr("Water subscription low"),
                         tr("The water subscription is particulary low (%1 m³), it is normal ?\n"
                            "You can disable the water subscription warning in parameters.")
                             .arg(ftod(ui->dsb_water_subscription->value())));
    return;
  }

  if (SETTINGS.get_invoice_warning("water_last_check").toBool()
      && ui->sb_last_water_check->value() <= SETTINGS.get_invoice_warning("water_last_check_threshold").toFloat()) {
    QMessageBox::warning(this, tr("Water last water check low"),
                         tr("The last water check is particulary low (%1 m³), it is normal ?\n"
                            "You can disable the last water check warning in parameters.")
                             .arg(ftod(ui->sb_last_water_check->value())));
    return;
  }

  if (SETTINGS.get_invoice_warning("water_check").toBool()
      && ui->sb_water_check->value() <= SETTINGS.get_invoice_warning("water_check_threshold").toFloat()) {
    QMessageBox::warning(this, tr("Water check low"),
                         tr("The water check is particulary low (%1 m³), it is normal ?\n"
                            "You can disable the water check warning in parameters.")
                             .arg(ftod(ui->sb_water_check->value())));
    return;
  }

  if (SETTINGS.get_general_dates() && SETTINGS.get_invoice_warning("dates").toBool()
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
  if (invoice->is_loaded() && invoice->is_dirty()) {
    invoice->update_record();
  } else if (invoice->is_dirty()) {
    invoice->insert_record();
  }

  if (manager) manager->refresh();
  close();
}


void W_Invoice_Creator::on_buttonBox_rejected()
{
  if (manager) manager->refresh();
  close();
}
