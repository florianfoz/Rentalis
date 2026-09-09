#ifndef W_RECEIPT_H
#define W_RECEIPT_H

#include <QWidget>

class W_Receipt_Manager;

namespace Ui
{
class W_Receipt;
}

class W_Receipt : public QWidget
{
  Q_OBJECT

public:
  explicit W_Receipt(W_Receipt_Manager* manager, int id);
  ~W_Receipt();

  W_Receipt_Manager* manager = nullptr;
  int                id      = -1;

  void refresh();
private slots:
  void on_b_edit_clicked();

  void on_b_delete_clicked();

private:
  Ui::W_Receipt* ui;
};

#endif // W_RECEIPT_H
