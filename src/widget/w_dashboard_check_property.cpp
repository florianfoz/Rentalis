#include "widget/w_dashboard_check_property.h"

#include "base.h"
#include "ui_w_dashboard_check_property.h"

#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>
#include <QtCharts>
#include <QtCore>
#include <QtGui>

W_Dashboard_check_property::W_Dashboard_check_property(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Dashboard_check_property)
{
  ui->setupUi(this);

  ui->cb_property->clear();

  if (auto q_property = DB_MANAGER.get_db()->all_records(ETable::properties)) {
    while (q_property->next()) {
      ui->cb_property->addItem(q_property->value("name").toString(), q_property->value("property_id").toInt());
    }
  }
  ui->cb_property->setCurrentIndex(0);

  ui->de_date_end->setDate(QDate::currentDate());
  ui->de_date_start->setDate(QDate::currentDate().addYears(-5));

  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

bool W_Dashboard_check_property::check_data()
{
  if (ui->de_date_start->date().toJulianDay() <= ui->de_date_end->date().toJulianDay()) return true;

  QString msg =
      QObject::tr("The entered dates are inconsistent; the end date [%1] must be later than the start date [%2].")
          .arg(ui->de_date_end->date().toString("dd/MM/yyyy"))
          .arg(ui->de_date_start->date().toString("dd/MM/yyyy"));

  QMessageBox::warning(nullptr, tr("Inconsistent Dates"), msg);

  return false;
}

void W_Dashboard_check_property::refresh()
{
  if (!check_data()) {

    return;
  }

  if (no_refresh) return;
  no_refresh = true;

  int     property_id    = ui->cb_property->currentData().toInt();
  QString start_date_str = ui->de_date_start->date().toString(Qt::ISODate);
  QString end_date_str   = ui->de_date_end->date().toString(Qt::ISODate);

  auto q_size = QSqlQuery(DB_MANAGER.get_db()->get_sql_db());
  q_size.prepare(R"(
        SELECT COUNT(*) as size
        FROM rents
        WHERE rents.property_id = :property_id
        AND rents.date >= :start_date
        AND rents.date < :end_date
    )");
  q_size.bindValue(":property_id", property_id);
  q_size.bindValue(":start_date", start_date_str);
  q_size.bindValue(":end_date", end_date_str);

  if (!q_size.exec() || !q_size.next()) return;

  int size = q_size.value("size").toInt();

  auto query = QSqlQuery(DB_MANAGER.get_db()->get_sql_db());
  query.prepare(R"(
        SELECT *
        FROM rents
        WHERE rents.property_id = :property_id
        AND rents.date >= :start_date
        AND rents.date < :end_date
    )");
  query.bindValue(":property_id", property_id);
  query.bindValue(":start_date", start_date_str);
  query.bindValue(":end_date", end_date_str);

  if (!Database::query_check(&query, "dashboard rents SELECT")) return;

  QVector<float> rents_data;
  rents_data.reserve(size);
  QVector<float> housing_aids_data;
  housing_aids_data.reserve(size);
  QVector<float> incomes;
  incomes.reserve(size);
  QStringList x_date_info;
  x_date_info.reserve(size);

  while (query.next()) {
    float rent        = query.value("rent").toFloat();
    float housing_aid = query.value("housing_aid").toFloat();
    QDate date        = query.value("date").toDate();

    rents_data.push_back(rent);
    housing_aids_data.push_back(housing_aid);
    incomes.push_back(rent + housing_aid);

    QString label = date.toString("yyyy - MM");
    x_date_info.push_back(label);
  }

  auto rents        = new QBarSet(tr("Rents"));
  auto housing_aids = new QBarSet(tr("Home Allowances"));

  for (int i = 0; i < rents_data.size(); ++i) {
    *rents << rents_data[i];
    *housing_aids << housing_aids_data[i];
  }

  auto series = new QStackedBarSeries();
  series->append(rents);
  series->append(housing_aids);

  auto chart = new QChart();
  chart->addSeries(series);
  chart->setAnimationOptions(QChart::SeriesAnimations);
  chart->setTheme(QChart::ChartThemeDark);

  chart->setMargins(QMargins(6, 6, 6, 6));
  chart->layout()->setContentsMargins(6, 6, 6, 6);

  // X axis
  QBarCategoryAxis* axisX = new QBarCategoryAxis();
  axisX->append(x_date_info);
  axisX->setLabelsAngle(90);
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  // Y axis
  // max y value
  float       max_y_val = incomes.empty() ? 0.0f : *std::max_element(incomes.begin(), incomes.end());
  QValueAxis* axisY     = new QValueAxis();
  axisY->setRange(0, max_y_val);
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  if (auto last_chart = ui->l_chart->takeAt(0)) {
    delete last_chart->widget();
    delete last_chart;
  }

  // Show
  auto view = new QChartView(chart);
  view->setRenderHint(QPainter::Antialiasing);
  ui->l_chart->addWidget(view);


  // tooltip
  connect(rents, &QBarSet::hovered, this, [=](bool status, int index) {
    if (status) {
      QString date  = x_date_info[index];
      float   value = rents_data[index];
      QToolTip::showText(QCursor::pos(), tr("Rent : %1\nDate : %2").arg(ftom(value)).arg(date));
    }
  });

  connect(housing_aids, &QBarSet::hovered, this, [=](bool status, int index) {
    if (status) {
      QString date  = x_date_info[index];
      float   value = housing_aids_data[index];
      QToolTip::showText(QCursor::pos(), tr("H. Allow. : %1\nDate : %2").arg(ftom(value)).arg(date));
    }
  });

  no_refresh = false;
}

W_Dashboard_check_property::~W_Dashboard_check_property()
{
  delete ui;
}

void W_Dashboard_check_property::on_de_date_start_userDateChanged(const QDate& date)
{
  refresh();
}


void W_Dashboard_check_property::on_de_date_end_userDateChanged(const QDate& date)
{
  refresh();
}


void W_Dashboard_check_property::on_cb_property_currentIndexChanged(int index)
{
  refresh();
}
