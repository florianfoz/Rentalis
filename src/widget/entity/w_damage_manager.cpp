#include "widget/entity/w_damage_manager.h"

#include "database/database.h"
#include "database/manager.h"
#include "ui_w_damage_manager.h"
#include "widget/entity/w_damage.h"
#include "widget/entity/w_damage_creator.h"

#include <QMessageBox>
#include <QSqlQuery>

W_Damage_Manager::W_Damage_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Damage_Manager)
{
  ui->setupUi(this);

  refresh();
}


void W_Damage_Manager::refresh()
{
  ui->tile_view->clear();

  for (auto id : Damage::all_records_id()) {
    auto* w = new W_Damage(id);
    ui->tile_view->add_widget(w);
  }
}


W_Damage_Manager::~W_Damage_Manager()
{
  delete ui;
}

void W_Damage_Manager::on_b_new_damage_clicked()
{
  auto* w = new W_Damage_Creator(INVALID_ID);
  w->setModal(true);
  w->exec();
}
