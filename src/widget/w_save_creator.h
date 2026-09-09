#ifndef W_SAVE_CREATOR_H
#define W_SAVE_CREATOR_H

#include "widget/w_save_manager.h"

#include <QDialog>

class MainMenu;

namespace Ui
{
class W_Save_Creator;
}

class W_Save_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Save_Creator(W_Save_Manager* manager);
  explicit W_Save_Creator(MainMenu* menu);
  ~W_Save_Creator();

  W_Save_Manager* manager = nullptr;
  MainMenu*       menu    = nullptr;

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::W_Save_Creator* ui;
};

#endif // W_SAVE_CREATOR_H
