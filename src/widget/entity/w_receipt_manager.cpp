#include "widget/entity/w_receipt_manager.h"

#include "database/database.h"
#include "database/manager.h"
#include "ui_w_receipt_manager.h"
#include "widget/entity/w_receipt.h"
#include "widget/entity/w_receipt_creator.h"

#include <QMessageBox>
#include <QSqlQuery>

W_Receipt_Manager::W_Receipt_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Receipt_Manager)
{
  ui->setupUi(this);

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable table) {
    if (table == ETable::Receipt || table == ETable::NONE) refresh();
  });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

void W_Receipt_Manager::refresh()
{
  ui->tile_view->clear();

  for (const auto& id : Receipt::all_records_id()) {
    auto* w = new W_Receipt(id);
    ui->tile_view->add_widget(w);
  }
}


W_Receipt_Manager::~W_Receipt_Manager()
{
  delete ui;
}

void W_Receipt_Manager::on_b_new_receipt_clicked()
{
  auto* w = new W_Receipt_Creator();
  w->setModal(true);
  w->exec();
}
