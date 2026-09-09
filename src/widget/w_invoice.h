#ifndef W_INVOICE_H
#define W_INVOICE_H

#include <QWidget>

class W_Invoice_Manager;

namespace Ui
{
class W_Invoice;
}

class W_Invoice : public QWidget
{
  Q_OBJECT

public:
  W_Invoice(W_Invoice_Manager* manager, int id);
  ~W_Invoice();

  W_Invoice_Manager* manager;
  int                id             = -1;
  int                rent_id_linked = -1;

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
