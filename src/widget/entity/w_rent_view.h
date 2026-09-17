#ifndef W_RENT_VIEW_H
#define W_RENT_VIEW_H

#include "forward.h"

#include <QWidget>
#include <QtSql>


class Property;

namespace Ui
{
class W_Rent_View;
}

class W_Rent_View : public QWidget
{
  Q_OBJECT

public:
  W_Rent_View(qsizetype _property_id, int _year);
  ~W_Rent_View();

  int       year        = QDate::currentDate().year();
  int       col_month   = 0;
  qsizetype property_id = INVALID_ID;

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
