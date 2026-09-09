#ifndef W_RENT_HISTOGRAM_H
#define W_RENT_HISTOGRAM_H

#include <QWidget>

namespace Ui
{
class W_Rent_Histogram;
}

class W_Rent_Histogram : public QWidget
{
  Q_OBJECT

public:
  explicit W_Rent_Histogram(int year, int property_id);
  ~W_Rent_Histogram();

private:
  Ui::W_Rent_Histogram* ui;
};

#endif // W_RENT_HISTOGRAM_H
