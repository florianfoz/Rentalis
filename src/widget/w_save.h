#ifndef W_SAVE_H
#define W_SAVE_H

#include <QDateTime>
#include <QWidget>

class W_Save_Manager;
class Database;

namespace Ui
{
class W_Save;
}

class W_Save : public QWidget
{
  Q_OBJECT

public:
  explicit W_Save(W_Save_Manager* manager, const QString& _file_name);
  ~W_Save();

  W_Save_Manager* manager = nullptr;
  QString         file_name;

  bool no_refresh = false;
  void refresh();
private slots:

  void on_b_delete_clicked();

  void on_b_load_clicked();

private:
  Ui::W_Save* ui;
};

#endif // W_SAVE_H
