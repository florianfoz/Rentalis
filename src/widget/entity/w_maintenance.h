#ifndef W_MAINTENANCE_H
#define W_MAINTENANCE_H

#include "forward.h"

#include <QWidget>

namespace Ui
{
class W_Maintenance;
}

class W_Maintenance : public QWidget
{
  Q_OBJECT

public:
  explicit W_Maintenance(qsizetype id);
  ~W_Maintenance();

  qsizetype id = INVALID_ID;

  void refresh();
private slots:
  void on_b_edit_clicked();

  void on_b_delete_clicked();

private:
  Ui::W_Maintenance* ui;
};

#endif // W_MAINTENANCE_H
