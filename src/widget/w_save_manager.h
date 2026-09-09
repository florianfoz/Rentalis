#ifndef W_SAVE_MANAGER_H
#define W_SAVE_MANAGER_H

#include <QWidget>

namespace Ui
{
class W_Save_Manager;
}

class W_Save_Manager : public QWidget
{
  Q_OBJECT

public:
  explicit W_Save_Manager(QWidget* parent = nullptr);
  ~W_Save_Manager();

  void refresh();
private slots:
  void on_b_new_save_clicked();

  void on_b_open_save_folder_clicked();

private:
  Ui::W_Save_Manager* ui;
};

#endif // W_SAVE_MANAGER_H
