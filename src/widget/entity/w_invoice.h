#ifndef W_INVOICE_H
#define W_INVOICE_H

#include "forward.h"

#include <QWidget>


namespace Ui
{
class W_Invoice;
}

class W_Invoice : public QWidget
{
  Q_OBJECT

public:
  W_Invoice(qsizetype id);
  ~W_Invoice();

  qsizetype id             = INVALID_ID;
  qsizetype rent_id_linked = INVALID_ID;

  void refresh();
private slots:
  void on_b_delete_clicked();

  void on_b_edit_clicked();

  void on_b_print_clicked();

  void on_b_send_clicked();

private:
  Ui::W_Invoice* ui;
};

#endif // W_INVOICE_H
