#include "widget/w_invoice_manager.h"

#include "base.h"
#include "ui_w_invoice_manager.h"
#include "widget/w_invoice.h"
#include "widget/w_invoice_creator.h"

#include <QMessageBox>
#include <QSqlQuery>

W_Invoice_Manager::W_Invoice_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Invoice_Manager)
{
  ui->setupUi(this);

  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

void W_Invoice_Manager::refresh()
{
  ui->tile_view->clear();


  if (auto query = DB_MANAGER.get_db()->all_records(ETable::invoices)) {
    while (query->next()) {
      int  id      = query->value("invoice_id").toInt();
      auto invoice = new W_Invoice(this, id);
      ui->tile_view->add_widget(invoice);
    }
  }
}


W_Invoice_Manager::~W_Invoice_Manager()
{
  delete ui;
}

void W_Invoice_Manager::on_b_new_invoice_clicked()
{
  auto invoice_creator = new W_Invoice_Creator(this, -1);
  invoice_creator->setModal(true);
  invoice_creator->exec();
}
