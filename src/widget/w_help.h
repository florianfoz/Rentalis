#ifndef W_HELP_H
#define W_HELP_H

#include <QDialog>

namespace Ui
{
class W_Help;
}

class W_Help : public QDialog
{
  Q_OBJECT

public:
  explicit W_Help(QWidget* parent = nullptr);
  ~W_Help();


private:
  Ui::W_Help* ui;
};

#endif // W_HELP_H
