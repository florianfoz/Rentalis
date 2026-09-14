#ifndef WELCOME_H
#define WELCOME_H

#include <QDialog>

namespace Ui
{
class W_Welcome;
}

class W_Welcome : public QDialog
{
  Q_OBJECT

public:
  explicit W_Welcome(QWidget* parent = nullptr);
  ~W_Welcome();


private:
  Ui::W_Welcome* ui;
};

#endif // WELCOME_H