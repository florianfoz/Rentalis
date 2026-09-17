#ifndef W_LANDLORD_CREATOR_H
#define W_LANDLORD_CREATOR_H

#include "entity/landlord.h"

#include <QDialog>

namespace Ui
{
class W_Landlord_Creator;
}

class W_Landlord_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Landlord_Creator(qsizetype id = INVALID_ID);
  ~W_Landlord_Creator();

  Landlord landlord;

  QString icon_type;
  QString singing_type;

  void clear();
  void populate_ui();
  void inject_data();
private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

  void on_b_icon_clicked();

  void on_b_singing_clicked();

private:
  Ui::W_Landlord_Creator* ui;
};

#endif // W_LANDLORD_CREATOR_H
