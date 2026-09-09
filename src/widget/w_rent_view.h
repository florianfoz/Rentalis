#ifndef W_RENT_VIEW_H
#define W_RENT_VIEW_H

#include <QWidget>
#include <QtSql>


class Property;
class W_Rent_Manager;

namespace Ui
{
class W_Rent_View;
}

class W_Rent_View : public QWidget
{
  Q_OBJECT

public:
  W_Rent_View(W_Rent_Manager* _manager, int _property_id, int _year);
  ~W_Rent_View();

  W_Rent_Manager* manager;

  int year        = QDate::currentDate().year();
  int col_month   = 0;
  int property_id = -1;

  void refresh();

  void set_signal_block(bool block);
private slots:
  void on_sb_year_dateChanged(const QDate& date);

  void on_cb_property_currentIndexChanged(int index);

  void on_b_graph_mode_clicked();

private:
  Ui::W_Rent_View* ui;
};

#endif // W_RENT_VIEW_H
