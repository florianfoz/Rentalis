#ifndef W_INVOICE_MANAGER_H
#define W_INVOICE_MANAGER_H

#include <QWidget>

namespace Ui
{
class W_Invoice_Manager;
}

class W_Invoice_Manager : public QWidget
{
  Q_OBJECT

public:
  explicit W_Invoice_Manager(QWidget* parent = nullptr);
  ~W_Invoice_Manager();

  void refresh();

private slots:
  void on_b_new_invoice_clicked();

private:
  Ui::W_Invoice_Manager* ui;
};

#endif // W_INVOICE_MANAGER_H
