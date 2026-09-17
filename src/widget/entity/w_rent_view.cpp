#include "widget/entity/w_rent_view.h"

#include "base.h"
#include "database/manager.h"
#include "entity/property.h"
#include "entity/rent.h"
#include "ui_w_rent_view.h"
#include "widget/entity/w_rent.h"
#include "widget/entity/w_rent_histogram.h"
#include "widget/entity/w_rent_manager.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTextStream>
#include <QToolTip>
#include <QtSql>


W_Rent_View::W_Rent_View(qsizetype _property_id, int _year)
  : year(_year)
  , property_id(_property_id)
  , ui(new Ui::W_Rent_View)
{
  ui->setupUi(this);


  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable table) {
    if (table == ETable::Rent || table == ETable::Property || table == ETable::Tenant || table == ETable::NONE)
      refresh();
  });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
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

  ui->cb_property->clear();

  for (const auto& rec : Property::all_records()) {
    QString display_name = rec.name + " (" + EPropertyType_to_str(rec.property_type) + ")";
    ui->cb_property->addItem(display_name, rec.id);
  }

  {
    int index = ui->cb_property->findData(property_id);
    ui->cb_property->setCurrentIndex(index);
  }
  ui->sb_year->setDate(QDate(year, 1, 1));

  while (auto* item = ui->l_rents->takeAt(0)) {
    if (auto* widget = item->widget()) widget->deleteLater();
    delete item;
  }

  while (auto* item = ui->l_chart->takeAt(0)) {
    if (auto* widget = item->widget()) widget->deleteLater();
    delete item;
  }


  for (auto month : EMonth_all) {
    if (month == EMonth::NONE) continue;
    QDate date      = QDate(year, static_cast<int>(month), 1);
    Rent  rent_inst = Rent::load_from_property(property_id, date);

    if (rent_inst) {
      auto* w = new W_Rent(rent_inst.id);
      ui->l_rents->addWidget(w);
    } else {
      auto* w = new W_Rent(date, property_id);
      ui->l_rents->addWidget(w);
    }
  }

  auto [trent, taid, tcharge, th_waste] = Rent::year_sum(year, property_id);

  // DEPECRATED
  ui->le_abattement->setText(ftom((trent + taid) /** manager->abattement_rate */));
  ui->le_declaration->setText(ftom((trent + taid) /* * (1 - manager->abattement_rate )*/));

  ui->le_tot_rent->setText(ftom(trent));
  ui->le_tot_aid->setText(ftom(taid));
  ui->le_tot_charges->setText(ftom(tcharge));
  ui->le_tot_wastes->setText(ftom(th_waste));


  if (auto* last_chart = ui->l_chart->takeAt(0)) {
    if (auto* widget = last_chart->widget()) widget->deleteLater();
    delete last_chart;
  }

  auto* w = new W_Rent_Histogram(year, property_id);
  ui->l_chart->addWidget(w);

  set_signal_block(false);
}

void W_Rent_View::on_sb_year_dateChanged(const QDate& date)
{
  year = date.year();
  refresh();
}

void W_Rent_View::on_cb_property_currentIndexChanged(int index)
{
  property_id = ui->cb_property->currentData().toInt();
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
