#ifndef W_DASHBOARD_CHECK_RENTS_DISTRIBUTION_H
#define W_DASHBOARD_CHECK_RENTS_DISTRIBUTION_H

#include <QWidget>

namespace Ui
{
class W_Dashboard_check_rents_distribution;
}

class W_Dashboard_check_rents_distribution : public QWidget
{
  Q_OBJECT

public:
  explicit W_Dashboard_check_rents_distribution(QWidget* parent = nullptr);
  ~W_Dashboard_check_rents_distribution();

  bool check_data();
  void refresh();

  bool no_refresh = false;

private slots:
  void on_de_date_start_userDateChanged(const QDate& date);

  void on_de_date_end_userDateChanged(const QDate& date);

  void on_lw_properties_itemSelectionChanged();

  void on_lw_properties_clicked(const QModelIndex& index);

private:
  Ui::W_Dashboard_check_rents_distribution* ui;
};

#endif // W_DASHBOARD_CHECK_RENTS_DISTRIBUTION_H
