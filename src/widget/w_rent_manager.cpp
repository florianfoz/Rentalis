#include "widget/w_rent_manager.h"

#include "database/manager.h"
#include "ui_w_rent_manager.h"
#include "widget/w_rent_view.h"

#include <QDesktopServices>
#include <QObject>
#include <QUrl>

W_Rent_Manager::W_Rent_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Rent_Manager)
{
  ui->setupUi(this);

  refresh();

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  // default views : 1, 2
  for (int i = 1; i < 3; ++i) {
    auto* rent = new W_Rent_View(this, i, QDate::currentDate().year());
    ui->layout_container->addWidget(rent);
  }
}

W_Rent_Manager::~W_Rent_Manager()
{
  delete ui;
}


void W_Rent_Manager::on_b_new_rent_view_clicked()
{
  auto* rent = new W_Rent_View(this, 1, QDate::currentDate().year());
  ui->layout_container->addWidget(rent);
}


void W_Rent_Manager::on_b_purge_rent_view_clicked()
{
  while (auto* item = ui->layout_container->takeAt(0)) {
    delete item->widget();
    delete item;
  }
}


void W_Rent_Manager::refresh()
{
  for (auto* item : ui->layout_container->children()) {
    if (auto* ptr = qobject_cast<W_Rent_View*>(item)) {
      ptr->refresh();
    }
  }
}
