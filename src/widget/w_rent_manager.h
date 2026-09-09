#ifndef W_RENT_MANAGER_H
#define W_RENT_MANAGER_H

#include <QDate>
#include <QWidget>

class W_Rent_View;

namespace Ui
{
class W_Rent_Manager;
}

class W_Rent_Manager : public QWidget
{
  Q_OBJECT

public:
  explicit W_Rent_Manager(QWidget* parent = nullptr);
  ~W_Rent_Manager();

  int   fiscal_year     = QDate::currentDate().year();
  float abattement_rate = 0.3f;

  void refresh();

private slots:
  void on_b_new_rent_view_clicked();

  void on_b_purge_rent_view_clicked();

private:
  Ui::W_Rent_Manager* ui;
};

#endif // W_RENT_MANAGER_H
