#ifndef W_INVOICE_FORM_H
#define W_INVOICE_FORM_H

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

  int invoice_id = -1;
  int rent_id    = -1;

private:
  Ui::W_Invoice_Form* ui;
};

#endif // W_INVOICE_FORM_H
