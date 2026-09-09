#include "widget/w_property_manager.h"

#include "base.h"
#include "ui_w_property_manager.h"
#include "widget/w_property.h"
#include "widget/w_property_creator.h"

#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>

W_Property_Manager::W_Property_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Property_Manager)
{
  ui->setupUi(this);

  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

W_Property_Manager::~W_Property_Manager()
{
  delete ui;
}

void W_Property_Manager::refresh()
{
  ui->tile_view->clear();

  QSqlQuery query(DB_MANAGER.get_db()->get_sql_db());
  if (query.exec("SELECT * FROM properties")) {
    while (query.next()) {
      int  id   = query.value("property_id").toInt();
      auto elem = new W_Property(this, id);
      ui->tile_view->add_widget(elem);
    }
  } else {
    QMessageBox::critical(this, tr("Search Failed"), tr("The property search has failed !"));
  }
}

void W_Property_Manager::on_b_new_clicked()
{
  auto cre = W_Property_Creator(this, -1);
  cre.setModal(true);
  cre.exec();
}
