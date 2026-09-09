#include "widget/w_rent_histogram.h"

#include "base.h"
#include "rent.h"
#include "ui_w_rent_histogram.h"

#include <QtCharts>
#include <QtCore>
#include <QtGui>

W_Rent_Histogram::W_Rent_Histogram(int year, int property_id)
  : ui(new Ui::W_Rent_Histogram)
{
  ui->setupUi(this);

  auto rents        = new QBarSet(tr("rent"));
  auto housing_aids = new QBarSet(tr("h. aid"));

  auto           result = DB_MANAGER.get_db()->find_rents_on_year(year, property_id);
  QVector<float> rents_data(12);
  QVector<float> housing_aids_data(12);
  QVector<float> incomes(12);
  for (auto [month, rent_id] : result) {
    auto index               = month - 1;
    auto rent                = Rent(rent_id);
    rents_data[index]        = rent.get_rent();
    housing_aids_data[index] = rent.get_housing_aid();
    incomes[index]           = rent.get_rent() + rent.get_housing_aid();
  }

  for (int i = 0; i < 12; ++i) {
    *rents << rents_data[i];
    *housing_aids << housing_aids_data[i];
  }

  auto series = new QStackedBarSeries();
  series->append(rents);
  series->append(housing_aids);

  auto chart = new QChart();
  chart->addSeries(series);
  chart->setAnimationOptions(QChart::SeriesAnimations);
  chart->legend()->setAlignment(Qt::AlignLeft);
  chart->setTheme(QChart::ChartThemeDark);

  // X axis
  QStringList months;
  for (auto& month : all_months) {
    months << EMonth_to_str(month);
  }

  QBarCategoryAxis* axisX = new QBarCategoryAxis();
  axisX->append(months);
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  // Y axis
  // max y value
  float       max_y_val = *std::max_element(incomes.begin(), incomes.end());
  QValueAxis* axisY     = new QValueAxis();
  axisY->setRange(0, max_y_val);
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  // Show
  auto view = new QChartView(chart);
  view->setRenderHint(QPainter::Antialiasing);
  ui->gridLayout->addWidget(view);

  connect(rents, &QBarSet::hovered, this, [=](bool status, int index) {
    if (status) {
      QString date  = months[index];
      float   value = rents_data[index];
      QToolTip::showText(QCursor::pos(), tr("Rent : %1\nDate : %2").arg(ftom(value)).arg(date));
    }
  });

  connect(housing_aids, &QBarSet::hovered, this, [=](bool status, int index) {
    if (status) {
      QString date  = months[index];
      float   value = housing_aids_data[index];
      QToolTip::showText(QCursor::pos(), tr("H. Allow. : %1\nDate : %2").arg(ftom(value)).arg(date));
    }
  });
}

W_Rent_Histogram::~W_Rent_Histogram()
{
  delete ui;
}
