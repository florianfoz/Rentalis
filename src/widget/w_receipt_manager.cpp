#include "widget/w_receipt_manager.h"

#include "database/database.h"
#include "database/manager.h"
#include "ui_w_receipt_manager.h"
#include "widget/w_receipt.h"
#include "widget/w_receipt_creator.h"

#include <QMessageBox>
#include <QSqlQuery>

W_Receipt_Manager::W_Receipt_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Receipt_Manager)
{
  ui->setupUi(this);

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

void W_Receipt_Manager::refresh()
{
  ui->tile_view->clear();

  if (auto query = Database_Manager::current_database()->all_records(ETable::Receipt)) {
    while (query->next()) {
      int   id      = query->value("receipt_id").toInt();
      auto* receipt = new W_Receipt(this, id);
      ui->tile_view->add_widget(receipt);
    }
  }
}


W_Receipt_Manager::~W_Receipt_Manager()
{
  delete ui;
}

void W_Receipt_Manager::on_b_new_receipt_clicked()
{
  auto* receipt_creator = new W_Receipt_Creator(this, -1);
  receipt_creator->setModal(true);
  receipt_creator->exec();
}
