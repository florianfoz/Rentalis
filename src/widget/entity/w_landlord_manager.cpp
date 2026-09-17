#include "widget/entity/w_landlord_manager.h"

#include "database/database.h"
#include "database/manager.h"
#include "ui_w_landlord_manager.h"
#include "widget/entity/w_landlord.h"
#include "widget/entity/w_landlord_creator.h"

#include <QMessageBox>
#include <QSqlQuery>

W_Landlord_Manager::W_Landlord_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Landlord_Manager)
{
  ui->setupUi(this);

  Database_Manager& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable table) {
    if (table == ETable::Landlord || table == ETable::NONE) refresh();
  });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

W_Landlord_Manager::~W_Landlord_Manager()
{
  delete ui;
}

void W_Landlord_Manager::refresh()
{
  ui->tile_view->clear();

  for (auto& id : Landlord::all_records_id()) {
    auto* w = new W_Landlord(id);
    ui->tile_view->add_widget(w);
  }
}

void W_Landlord_Manager::on_b_new_clicked()
{
  auto cre = W_Landlord_Creator();
  cre.setModal(true);
  cre.exec();
}
