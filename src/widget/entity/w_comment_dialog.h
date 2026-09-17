#ifndef W_COMMENT_DIALOG_H
#define W_COMMENT_DIALOG_H

#include "entity/rent.h"

#include <QDialog>

namespace Ui
{
class W_Comment_Dialog;
}

class W_Comment_Dialog : public QDialog
{
  Q_OBJECT

public:
  W_Comment_Dialog(int rent_id);

  ~W_Comment_Dialog();

  int  rent_id = INVALID_ID;
  Rent rent;

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::W_Comment_Dialog* ui;
};

#endif // W_COMMENT_DIALOG_H
