#include "widget/entity/w_rent_manager.h"

#include "database/manager.h"
#include "entity/property.h"
#include "ui_w_rent_manager.h"
#include "widget/entity/w_rent_view.h"

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

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable table) {
    if (table == ETable::Rent || table == ETable::Tenant || table == ETable::Property || table == ETable::NONE)
      refresh();
  });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  // default views
  ui->layout_container->addWidget(new W_Rent_View(INVALID_ID, QDate::currentDate().year()));
  ui->layout_container->addWidget(new W_Rent_View(INVALID_ID, QDate::currentDate().year()));
}

W_Rent_Manager::~W_Rent_Manager()
{
  delete ui;
}


void W_Rent_Manager::on_b_new_rent_view_clicked()
{
  auto* w = new W_Rent_View(INVALID_ID, QDate::currentDate().year());
  ui->layout_container->addWidget(w);
}


void W_Rent_Manager::on_b_purge_rent_view_clicked()
{
  while (auto* item = ui->layout_container->takeAt(0)) {
    if (auto* widget = item->widget()) widget->deleteLater();
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
