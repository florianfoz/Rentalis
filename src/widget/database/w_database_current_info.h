#ifndef W_DATABASE_CURRENT_INFO_H
#define W_DATABASE_CURRENT_INFO_H

#include <QWidget>

namespace Ui
{
class W_Database_Current_Info;
}

class W_Database_Current_Info : public QWidget
{
  Q_OBJECT

public:
  explicit W_Database_Current_Info(QWidget* parent = nullptr);
  ~W_Database_Current_Info();

  void refresh();

private:
  Ui::W_Database_Current_Info* ui;
};

#endif // W_DATABASE_CURRENT_INFO_H
