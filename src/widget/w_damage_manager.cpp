#include "widget/w_damage_manager.h"

#include "base.h"
#include "ui_w_damage_manager.h"
#include "widget/w_damage.h"
#include "widget/w_damage_creator.h"

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

  if (auto query = DB_MANAGER.get_db()->all_records(ETable::damages)) {
    while (query->next()) {
      int  id     = query->value("damage_id").toInt();
      auto damage = new W_Damage(this, id);
      ui->tile_view->add_widget(damage);
    }
  }
}


W_Damage_Manager::~W_Damage_Manager()
{
  delete ui;
}

void W_Damage_Manager::on_b_new_damage_clicked()
{
  auto damage_creator = new W_Damage_Creator(this, -1);
  damage_creator->setModal(true);
  damage_creator->exec();
}
