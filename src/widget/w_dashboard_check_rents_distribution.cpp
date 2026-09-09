#include "widget/w_dashboard_check_rents_distribution.h"

#include "base.h"
#include "property.h"
#include "ui_w_dashboard_check_rents_distribution.h"

#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>
#include <QtCharts>
#include <QtCore>
#include <QtGui>

W_Dashboard_check_rents_distribution::W_Dashboard_check_rents_distribution(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Dashboard_check_rents_distribution)
{
  ui->setupUi(this);

  ui->lw_properties->clear();

  if (auto properties = DB_MANAGER.get_db()->all_records(ETable::properties)) {
    while (properties->next()) {
      auto             name = properties->value("name").toString();
      auto             id   = properties->value("property_id").toInt();
      QListWidgetItem* item = new QListWidgetItem(name, ui->lw_properties);
      item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
      item->setData(Qt::UserRole, id);
      item->setCheckState(Qt::Checked);
    }
  }

  no_refresh = true;

  ui->de_date_end->setDate(QDate::currentDate());
  ui->de_date_start->setDate(QDate::currentDate().addYears(-5));

  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  no_refresh = false;
  refresh();
}


W_Dashboard_check_rents_distribution::~W_Dashboard_check_rents_distribution()
{
  delete ui;
}


bool W_Dashboard_check_rents_distribution::check_data()
{
  if (ui->de_date_start->date().toJulianDay() <= ui->de_date_end->date().toJulianDay()) return true;


  QString msg =
      QObject::tr("The entered dates are inconsistent; the end date [%1] must be later than the start date [%2].")
          .arg(ui->de_date_end->date().toString("dd/MM/yyyy"))
          .arg(ui->de_date_start->date().toString("dd/MM/yyyy"));

  QMessageBox::warning(nullptr, tr("Inconsistent Dates"), msg);

  return false;
}

void W_Dashboard_check_rents_distribution::refresh()
{
  if (!check_data()) {

    return;
  }

  if (no_refresh) return;
  no_refresh = true;

  QList<int> selected_ids;

  for (int i = 0; i < ui->lw_properties->count(); ++i) {
    auto item = ui->lw_properties->item(i);
    if (item->checkState() == Qt::Checked) {
      selected_ids << item->data(Qt::UserRole).toInt();
    }
  }

  if (selected_ids.empty()) {
    no_refresh = false;
    return;
  }

  QPieSeries* income_series = new QPieSeries();

  for (auto build_id : selected_ids) {
    QString start_date_str = ui->de_date_start->date().toString(Qt::ISODate);
    QString end_date_str   = ui->de_date_end->date().toString(Qt::ISODate);

    auto query = QSqlQuery(DB_MANAGER.get_db()->get_sql_db());
    query.prepare(R"(
            SELECT SUM(rents.rent) as sum_rent, SUM(rents.housing_aid) as sum_housing_aid
            FROM rents
            WHERE rents.property_id = :property_id
            AND rents.date >= :start_date
            AND rents.date < :end_date;
        )");
    query.bindValue(":property_id", build_id);
    query.bindValue(":start_date", start_date_str);
    query.bindValue(":end_date", end_date_str);

    if (!Database::query_check(&query, "dashboard rents SELECT") || !query.next()) {
      no_refresh = false;
      return;
    }

    float sum_rent        = query.value("sum_rent").toFloat();
    float sum_housing_aid = query.value("sum_housing_aid").toFloat();
    float sum_income      = sum_rent + sum_housing_aid;

    auto name = Property(build_id).get_name();

    qDebug() << name << ": rent: " << sum_rent << ", housing_aid:" << sum_housing_aid;

    income_series->append(name, sum_income);
  }


  QChart* chart = new QChart();
  chart->addSeries(income_series);
  chart->setTheme(QChart::ChartThemeDark);

  chart->setMargins(QMargins(0, 0, 0, 0));
  chart->layout()->setContentsMargins(0, 0, 0, 0);

  if (auto last_chart = ui->l_chart->takeAt(0)) {
    delete last_chart->widget();
    delete last_chart;
  }

  // Show
  QChartView* view = new QChartView(chart);
  view->setRenderHint(QPainter::Antialiasing);


  ui->l_chart->addWidget(view);


  // tooltip
  for (auto slice : income_series->slices()) {
    connect(slice, &QPieSlice::hovered, this, [slice](bool state) {
      if (state) {
        QString name  = slice->label();
        double  value = slice->value();

        QToolTip::showText(QCursor::pos(), QString("%1 : %2").arg(name).arg(ftom(value)));
      }
    });
  }

  no_refresh = false;
}

void W_Dashboard_check_rents_distribution::on_de_date_start_userDateChanged(const QDate& date)
{
  refresh();
}


void W_Dashboard_check_rents_distribution::on_de_date_end_userDateChanged(const QDate& date)
{
  refresh();
}


void W_Dashboard_check_rents_distribution::on_lw_properties_itemSelectionChanged()
{
  refresh();
}


void W_Dashboard_check_rents_distribution::on_lw_properties_clicked(const QModelIndex& index)
{
  refresh();
}
