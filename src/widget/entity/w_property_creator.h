#ifndef W_PROPERTY_CREATOR_H
#define W_PROPERTY_CREATOR_H

#include "entity/property.h"

#include <QDialog>

class Property;

enum class EMode : uint8_t;

namespace Ui
{
class W_Property_Creator;
}

class W_Property_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Property_Creator(qsizetype id = INVALID_ID);
  ~W_Property_Creator();

  Property property;

  QString icon_type;

  void clear();
  void populate_ui();
  void inject_data();

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

  void on_b_icon_clicked();


private:
  Ui::W_Property_Creator* ui;
};

#endif // W_PROPERTY_CREATOR_H
