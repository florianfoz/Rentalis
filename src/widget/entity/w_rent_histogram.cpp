#include "widget/entity/w_rent_histogram.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "entity/rent.h"
#include "ui_w_rent_histogram.h"

#include <QtCharts>
#include <QtCore>
#include <QtGui>
#include <algorithm>

W_Rent_Histogram::W_Rent_Histogram(int year, qsizetype property_id)
  : ui(new Ui::W_Rent_Histogram)
{
  ui->setupUi(this);

  auto* rents        = new QBarSet(tr("rent"));
  auto* housing_aids = new QBarSet(tr("h. aid"));

  auto           result = Rent::find_by_year_property(year, property_id);
  QVector<float> rents_data(12);
  QVector<float> housing_aids_data(12);
  QVector<float> incomes(12);
  for (auto [month, rent_id] : result) {
    auto index               = month - 1;
    auto rent                = Rent::read_record(rent_id);
    rents_data[index]        = rent.rent;
    housing_aids_data[index] = rent.housing_aid;
    incomes[index]           = rent.rent + rent.housing_aid;
  }

  for (int i = 0; i < 12; ++i) {
    *rents << rents_data[i];
    *housing_aids << housing_aids_data[i];
  }

  auto* series = new QStackedBarSeries();
  series->append(rents);
  series->append(housing_aids);

  auto* chart = new QChart();
  chart->addSeries(series);
  chart->setAnimationOptions(QChart::SeriesAnimations);
  chart->legend()->setAlignment(Qt::AlignLeft);
  chart->setTheme(QChart::ChartThemeDark);

  // X axis
  QStringList months;
  for (const auto& month : EMonth_names) {
    months << month;
  }

  auto* axisX = new QBarCategoryAxis();
  axisX->append(months);
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  // Y axis
  // max y value
  float max_y_val = *std::ranges::max_element(incomes);
  auto* axisY     = new QValueAxis();
  axisY->setRange(0, max_y_val);
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  // Show
  auto* view = new QChartView(chart);
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
