#ifndef W_RECEIPT_FORM_H
#define W_RECEIPT_FORM_H

#include <QWidget>

namespace Ui
{
class W_Receipt_Form;
}

class W_Receipt_Form : public QWidget
{
  Q_OBJECT

public:
  explicit W_Receipt_Form(int receipt_id, int rent_id);
  ~W_Receipt_Form();

  int receipt_id = -1;
  int rent_id    = -1;

private:
  Ui::W_Receipt_Form* ui;
};

#endif // W_RECEIPT_FORM_H
