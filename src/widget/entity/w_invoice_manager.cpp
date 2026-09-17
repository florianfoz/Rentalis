#include "widget/entity/w_invoice_manager.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "ui_w_invoice_manager.h"
#include "widget/entity/w_invoice.h"
#include "widget/entity/w_invoice_creator.h"

#include <QMessageBox>
#include <QSqlQuery>

W_Invoice_Manager::W_Invoice_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Invoice_Manager)
{
  ui->setupUi(this);

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable table) {
    if (table == ETable::Invoice || table == ETable::NONE) refresh();
  });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

void W_Invoice_Manager::refresh()
{
  ui->tile_view->clear();

  for (const auto& id : Invoice::all_records_id()) {
    auto* w = new W_Invoice(id);
    ui->tile_view->add_widget(w);
  }
}


W_Invoice_Manager::~W_Invoice_Manager()
{
  delete ui;
}

void W_Invoice_Manager::on_b_new_invoice_clicked()
{
  auto* w = new W_Invoice_Creator();
  w->setModal(true);
  w->exec();
}
