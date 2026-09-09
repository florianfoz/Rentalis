#ifndef W_DASHBOARD_CHECK_PROPERTY_H
#define W_DASHBOARD_CHECK_PROPERTY_H

#include <QWidget>

namespace Ui
{
class W_Dashboard_check_property;
}

class W_Dashboard_check_property : public QWidget
{
  Q_OBJECT

public:
  explicit W_Dashboard_check_property(QWidget* parent = nullptr);
  ~W_Dashboard_check_property();

  void refresh();
  bool check_data();

  bool no_refresh = false;

private slots:
  void on_de_date_start_userDateChanged(const QDate& date);

  void on_de_date_end_userDateChanged(const QDate& date);

  void on_cb_property_currentIndexChanged(int index);

private:
  Ui::W_Dashboard_check_property* ui;
};

#endif // W_DASHBOARD_CHECK_PROPERTY_H
