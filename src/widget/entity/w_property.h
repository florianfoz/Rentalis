#ifndef W_PROPERTY_H
#define W_PROPERTY_H

#include "forward.h"

#include <QWidget>

namespace Ui
{
class W_Property;
}

class W_Property : public QWidget
{
  Q_OBJECT

public:
  explicit W_Property(qsizetype id);
  ~W_Property();

  qsizetype id = INVALID_ID;

  void refresh();
private slots:
  void on_b_edit_clicked();

  void on_b_delete_clicked();

private:
  Ui::W_Property* ui;
};

#endif // W_PROPERTY_H
