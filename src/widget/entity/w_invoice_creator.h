#ifndef W_INVOICE_CREATOR_H
#define W_INVOICE_CREATOR_H

#include "entity/invoice.h"

#include <QDialog>

namespace Ui
{
class W_Invoice_Creator;
}

class W_Invoice_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Invoice_Creator(qsizetype id = INVALID_ID);
  ~W_Invoice_Creator();

  Invoice invoice;

  void clear();
  void populate_ui();
  void inject_data();
  void check_data();

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::W_Invoice_Creator* ui;
};

#endif // W_INVOICE_CREATOR_H
