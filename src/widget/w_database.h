#ifndef W_DATABASE_H
#define W_DATABASE_H

#include <QDateTime>
#include <QWidget>

class W_Database_Manager;
class Database;

namespace Ui
{
class W_Database;
}

class W_Database : public QWidget
{
  Q_OBJECT

public:
  explicit W_Database(W_Database_Manager* manager, const QString& _save_path);
  ~W_Database();

  W_Database_Manager* manager = nullptr;
  QString             save_path;

  bool no_refresh = false;
  void refresh();
private slots:

  void on_b_delete_clicked();
  void on_b_load_clicked();

private:
  Ui::W_Database* ui;
};

#endif // W_DATABASE_H
