#ifndef W_INVOICE_FORM_H
#define W_INVOICE_FORM_H

#include "entity/invoice.h"
#include "entity/rent.h"

#include <QWidget>

namespace Ui
{
class W_Invoice_Form;
}

class W_Invoice_Form : public QWidget
{
  Q_OBJECT

public:
  explicit W_Invoice_Form(qsizetype invoice_id, qsizetype rent_id);
  ~W_Invoice_Form();

  Invoice invoice;
  Rent    rent;

private:
  Ui::W_Invoice_Form* ui;
};

#endif // W_INVOICE_FORM_H
