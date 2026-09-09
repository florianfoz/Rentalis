#ifndef W_RECEIPT_MANAGER_H
#define W_RECEIPT_MANAGER_H

#include <QWidget>

namespace Ui
{
class W_Receipt_Manager;
}

class W_Receipt_Manager : public QWidget
{
  Q_OBJECT

public:
  explicit W_Receipt_Manager(QWidget* parent = nullptr);
  ~W_Receipt_Manager();

  void refresh();

private slots:
  void on_b_new_receipt_clicked();

private:
  Ui::W_Receipt_Manager* ui;
};

#endif // W_RECEIPT_MANAGER_H
