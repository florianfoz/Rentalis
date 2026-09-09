#include "widget/w_dashboard.h"

#include "base.h"
#include "invoice.h"
#include "property.h"
#include "receipt.h"
#include "tenant.h"
#include "ui_w_dashboard.h"

#include <QSqlQuery>

W_Dashboard::W_Dashboard(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Dashboard)
{
  ui->setupUi(this);

  no_refresh = false;

  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

void W_Dashboard::refresh()
{
  if (no_refresh) return;
  no_refresh = true;

  refresh_todo();
  refresh_tw_year_incomes();
  refresh_tw_tenants();

  no_refresh = false;
}

void W_Dashboard::refresh_todo()
{
  QString out_str;

  // ------------ rents ------------
  {
    QSqlQuery query(DB_MANAGER.get_db()->get_sql_db());
    query.prepare(R"(
            SELECT b.property_id,
                   r.tenant_id,
                   CASE
                       WHEN EXISTS (
                           SELECT 1
                           FROM rents r2
                           WHERE r2.property_id = b.property_id
                             AND r2.tenant_id = r.tenant_id
                             AND strftime('%Y-%m', r2.date) = strftime('%Y-%m', 'now', '-1 month')
                       )
                       THEN 1
                       ELSE 0
                   END AS has_rent_last_month
            FROM properties b
            LEFT JOIN rents r ON r.rent_id = (
                SELECT r3.rent_id
                FROM rents r3
                WHERE r3.property_id = b.property_id
                ORDER BY r3.date DESC
                LIMIT 1
            )
            ORDER BY b.property_id;
        )");
    query.exec();

    out_str += "<p><b>" + tr("Rent(s) to be recorded last month:") + "</b></p>";
    out_str += "<ul>";

    while (query.next()) {
      int  property_id = query.value("property_id").toInt();
      int  tenant_id   = query.value("tenant_id").toInt();
      bool hasRent     = query.value("has_rent_last_month").toBool();

      if (tenant_id > 0 && !hasRent) {
        auto build = Property(property_id);
        out_str += "<li>" + build.get_address() + "</li>";
      }
    }

    out_str += "</ul>";
  }

  // ------------ invoices -------------
  {
    QSqlQuery query(DB_MANAGER.get_db()->get_sql_db());
    query.prepare(R"(
            SELECT b.property_id,
                   r.tenant_id,
                   CASE
                       WHEN EXISTS (
                           SELECT 1
                           FROM invoices i
                           WHERE i.tenant_id = r.tenant_id
                             AND date('now', '-1 month') BETWEEN i.start_date AND i.end_date
                       )
                       THEN 1
                       ELSE 0
                   END AS has_invoice_last_month
            FROM properties b
            LEFT JOIN rents r ON r.rent_id = (
                SELECT r3.rent_id
                FROM rents r3
                WHERE r3.property_id = b.property_id
                ORDER BY r3.date DESC
                LIMIT 1
            )
            ORDER BY b.property_id;
        )");
    query.exec();

    out_str += "<p><b>" + tr("Invoice(s) to print last month:") + "</b></p>";
    out_str += "<ul>";

    while (query.next()) {
      int  property_id = query.value("property_id").toInt();
      int  tenant_id   = query.value("tenant_id").toInt();
      bool hasInvoice  = query.value("has_invoice_last_month").toBool();

      if (tenant_id > 0 && !hasInvoice) {
        auto build  = Property(property_id);
        auto tenant = Tenant(tenant_id);
        out_str +=
            "<li>" + tr("tenant: ") + tenant.get_full_name() + tr(" - property: ") + build.get_address() + "</li>";
      }
    }

    out_str += "</ul>";
  }

  // ------------ receipts ------------
  {
    QSqlQuery query(DB_MANAGER.get_db()->get_sql_db());
    query.prepare(R"(
            SELECT b.property_id,
                   r.tenant_id,
                   CASE
                       WHEN EXISTS (
                           SELECT 1
                           FROM receipts rc
                           WHERE rc.tenant_id = r.tenant_id
                             AND date('now', '-1 month') BETWEEN rc.start_date AND rc.end_date
                       )
                       THEN 1
                       ELSE 0
                   END AS has_receipt_last_month
            FROM properties b
            LEFT JOIN rents r ON r.rent_id = (
                SELECT r3.rent_id
                FROM rents r3
                WHERE r3.property_id = b.property_id
                ORDER BY r3.date DESC
                LIMIT 1
            )
            ORDER BY b.property_id;
        )");
    query.exec();

    out_str += "<p><b>" + tr("Receipt(s) to print last month:") + "</b></p>";
    out_str += "<ul>";

    while (query.next()) {
      int  property_id = query.value("property_id").toInt();
      int  tenant_id   = query.value("tenant_id").toInt();
      bool hasReceipt  = query.value("has_receipt_last_month").toBool();

      if (tenant_id > 0 && !hasReceipt) {
        auto build  = Property(property_id);
        auto tenant = Tenant(tenant_id);
        out_str +=
            "<li>" + tr("tenant: ") + tenant.get_full_name() + tr(" - property: ") + build.get_address() + "</li>";
      }
    }

    out_str += "</ul>";
  }

  ui->tb_todo->setText(out_str);
}


void W_Dashboard::refresh_tw_year_incomes()
{
  ui->tw_year_incomes->setRowCount(0);

  auto query = QSqlQuery(DB_MANAGER.get_db()->get_sql_db());
  query.prepare(R"(
        SELECT
            strftime("%Y", rents.date) AS year,
            SUM(rents.rent + rents.housing_aid) AS incomes
        FROM rents
        GROUP BY year
        ORDER BY year DESC;
    )");
  query.exec();

  float income_tot = 0.0f;

  int row = 0;
  while (query.next()) {
    auto year   = query.value("year").toString();
    auto income = query.value("incomes").toFloat();
    income_tot += income;

    QTableWidgetItem* l_year_item   = new QTableWidgetItem(year);
    QTableWidgetItem* l_income_item = new QTableWidgetItem(ftom(income));
    l_income_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    ui->tw_year_incomes->insertRow(row);
    ui->tw_year_incomes->setItem(row, 0, l_year_item);
    ui->tw_year_incomes->setItem(row, 1, l_income_item);

    row++;
  }

  ui->l_tot_year_incomes->setText(ftom(income_tot));


  ui->tw_year_incomes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void W_Dashboard::refresh_tw_tenants()
{
  ui->tw_tenants->setRowCount(0);

  auto query = QSqlQuery(DB_MANAGER.get_db()->get_sql_db());
  query.prepare(R"(
        SELECT property_id, tenant_id
        FROM (
            SELECT r.property_id, r.tenant_id,
                   ROW_NUMBER() OVER (PARTITION BY r.property_id ORDER BY r.date DESC) as rn
            FROM rents r
        ) sub
        WHERE rn = 1;
    )");
  query.exec();

  int row = 0;
  while (query.next()) {
    int property_id = query.value("property_id").toInt();
    int tenant_id   = query.value("tenant_id").toInt();

    if (tenant_id < 1 || property_id < 1) return;

    Property property(property_id);
    Tenant   tenant(tenant_id);

    if (!property.is_loaded() || !tenant.is_loaded()) return;

    QTableWidgetItem* l_address_item = new QTableWidgetItem(property.get_address());
    QTableWidgetItem* l_name_item    = new QTableWidgetItem(tenant.get_full_name());
    QTableWidgetItem* l_phone_item   = new QTableWidgetItem(tenant.get_phone());
    QTableWidgetItem* l_email_item   = new QTableWidgetItem(tenant.get_email());

    ui->tw_tenants->insertRow(row);

    ui->tw_tenants->setItem(row, 0, l_name_item);
    ui->tw_tenants->setItem(row, 1, l_phone_item);
    ui->tw_tenants->setItem(row, 2, l_email_item);
    ui->tw_tenants->setItem(row, 3, l_address_item);

    row++;
  }

  ui->tw_tenants->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

W_Dashboard::~W_Dashboard()
{
  delete ui;
}
