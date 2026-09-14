#ifndef W_INVOICE_FORM_H
#define W_INVOICE_FORM_H

#include "entities/invoice.h"
#include "entities/rent.h"

#include <QWidget>

namespace Ui
{
class W_Invoice_Form;
}

class W_Invoice_Form : public QWidget
{
  Q_OBJECT

public:
  explicit W_Invoice_Form(int invoice_id, int rent_id);
  ~W_Invoice_Form();

  Invoice invoice;
  Rent    rent;

private:
  Ui::W_Invoice_Form* ui;
};

#endif // W_INVOICE_FORM_H
