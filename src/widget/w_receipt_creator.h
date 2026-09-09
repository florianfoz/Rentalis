#ifndef W_RECEIPT_CREATOR_H
#define W_RECEIPT_CREATOR_H

#include <QDialog>

class W_Receipt_Manager;
class Receipt;

namespace Ui
{
class W_Receipt_Creator;
}

class W_Receipt_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Receipt_Creator(W_Receipt_Manager* manager, int id);
  ~W_Receipt_Creator();

  W_Receipt_Manager* manager = nullptr;
  Receipt*           receipt;

  void populate_ui();
  void clear();
  void inject_data();
  void update_ui();
private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();
  void on_cb_tenant_currentIndexChanged(int index);

  void on_de_date_start_userDateChanged(const QDate& date);

  void on_de_date_end_userDateChanged(const QDate& date);

private:
  Ui::W_Receipt_Creator* ui;
};

#endif // W_RECEIPT_CREATOR_H
