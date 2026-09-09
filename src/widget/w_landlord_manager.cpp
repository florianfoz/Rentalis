#include "widget/w_landlord_manager.h"

#include "base.h"
#include "ui_w_landlord_manager.h"
#include "widget/w_landlord.h"
#include "widget/w_landlord_creator.h"

#include <QMessageBox>
#include <QSqlQuery>

W_Landlord_Manager::W_Landlord_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Landlord_Manager)
{
  ui->setupUi(this);

  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });

  refresh();
}

W_Landlord_Manager::~W_Landlord_Manager()
{
  delete ui;
}

void W_Landlord_Manager::refresh()
{
  ui->tile_view->clear();

  QSqlQuery query(DB_MANAGER.get_db()->get_sql_db());
  if (query.exec("SELECT * FROM landlords")) {
    while (query.next()) {
      int  id       = query.value("landlord_id").toInt();
      auto landlord = new W_Landlord(this, id);
      ui->tile_view->add_widget(landlord);
    }
  } else {
    QMessageBox::critical(this, tr("Search Failed"), tr("The landlords search failed !"));
  }
}

void W_Landlord_Manager::on_b_new_clicked()
{
  auto cre = W_Landlord_Creator(this, -1);
  cre.setModal(true);
  cre.exec();
}
