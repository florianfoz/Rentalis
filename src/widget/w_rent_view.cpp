#include "widget/w_rent_view.h"

#include "base.h"
#include "property.h"
#include "rent.h"
#include "tenant.h"
#include "ui_w_rent_view.h"
#include "widget/w_rent.h"
#include "widget/w_rent_histogram.h"
#include "widget/w_rent_manager.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTextStream>
#include <QToolTip>
#include <QtSql>


W_Rent_View::W_Rent_View(W_Rent_Manager* _manager, int _property_id, int _year)
  : manager(_manager)
  , year(_year)
  , property_id(property_id)
  , ui(new Ui::W_Rent_View)
{
  ui->setupUi(this);

  set_signal_block(true);

  ui->cb_property->clear();

  if (auto properties = DB_MANAGER.get_db()->all_records(ETable::properties)) {
    while (properties->next()) {
      Property property(properties->value("property_id").toInt());
      QString  display_name = property.get_name() + " (" + EPropertyType_to_str(property.get_property_type()) + ")";
      ui->cb_property->addItem(display_name, property.get_id());
    }
  }

  int property_index = ui->cb_property->findData(property_id);
  ui->cb_property->setCurrentIndex(property_index);
  ui->sb_year->setDate(QDate(year, 1, 1));


  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();

  set_signal_block(false);
}

void W_Rent_View::set_signal_block(bool block)
{
  ui->cb_property->blockSignals(block);
  ui->sb_year->blockSignals(block);
}

W_Rent_View::~W_Rent_View()
{
  delete ui;
}

void W_Rent_View::refresh()
{
  set_signal_block(true);

  while (auto item = ui->l_rents->takeAt(0)) {
    if (item->widget()) delete item->widget();
    delete item;
  }

  while (auto item = ui->l_chart->takeAt(0)) {
    if (item->widget()) delete item->widget();
    delete item;
  }

  year        = ui->sb_year->date().year();
  property_id = ui->cb_property->currentData().toInt();


  for (auto month : all_months) {
    QDate month_date = QDate(year, static_cast<int>(month), 1);
    Rent  rent_inst  = Rent::load_from_property(property_id, month_date);

    if (rent_inst.is_loaded()) {
      W_Rent* w_rent = new W_Rent(manager, rent_inst.get_id(), month);
      ui->l_rents->addWidget(w_rent);
    } else {
      W_Rent* w_rent = new W_Rent(manager, -1, month);
      ui->l_rents->addWidget(w_rent);
    }
  }

  auto [trent, taid, tcharge, th_waste] = DB_MANAGER.get_db()->total_year_rent_sum(year, property_id);

  ui->le_abattement->setText(ftom((trent + taid) * manager->abattement_rate));
  ui->le_declaration->setText(ftom((trent + taid) * (1 - manager->abattement_rate)));

  ui->le_tot_rent->setText(ftom(trent));
  ui->le_tot_aid->setText(ftom(taid));
  ui->le_tot_charges->setText(ftom(tcharge));
  ui->le_tot_wastes->setText(ftom(th_waste));


  if (auto last_chart = ui->l_chart->takeAt(0)) {
    delete last_chart->widget();
    delete last_chart;
  }

  auto rent_chart = new W_Rent_Histogram(year, property_id);
  ui->l_chart->addWidget(rent_chart);

  set_signal_block(false);
}

void W_Rent_View::on_sb_year_dateChanged(const QDate& date)
{
  refresh();
}

void W_Rent_View::on_cb_property_currentIndexChanged(int index)
{
  refresh();
}

void W_Rent_View::on_b_graph_mode_clicked()
{
  // table mode -> switch to graph mode
  if (ui->sw_view->currentIndex() == 0) {
    ui->sw_view->setCurrentIndex(1);
    ui->b_graph_mode->setText(tr("Table"));
    // table icon
    ui->b_graph_mode->setIcon(QIcon(":/assets/accessories-calculator.svg"));
  }
  // graĥ mode -> switch to table mode
  else {
    ui->sw_view->setCurrentIndex(0);
    ui->b_graph_mode->setText(tr("Chart"));
    // graph icon
    ui->b_graph_mode->setIcon(QIcon(":/assets/report.svg"));
  }
}
