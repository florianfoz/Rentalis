#include "widget/entity/w_property_manager.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "ui_w_property_manager.h"
#include "widget/entity/w_property.h"
#include "widget/entity/w_property_creator.h"

#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>

W_Property_Manager::W_Property_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Property_Manager)
{
  ui->setupUi(this);

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable table) {
    if (table == ETable::Property || table == ETable::NONE) refresh();
  });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

W_Property_Manager::~W_Property_Manager()
{
  delete ui;
}

void W_Property_Manager::refresh()
{
  ui->tile_view->clear();

  for (auto id : Property::all_records_id()) {
    auto* w = new W_Property(id);
    ui->tile_view->add_widget(w);
  }
}

void W_Property_Manager::on_b_new_clicked()
{
  auto cre = W_Property_Creator();
  cre.setModal(true);
  cre.exec();
}
