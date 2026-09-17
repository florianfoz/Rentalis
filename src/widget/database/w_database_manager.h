#ifndef W_DATABASE_MANAGER_H
#define W_DATABASE_MANAGER_H

#include <QWidget>

namespace Ui
{
class W_Database_Manager;
}

class W_Database_Manager : public QWidget
{
  Q_OBJECT

public:
  explicit W_Database_Manager(QWidget* parent = nullptr);
  ~W_Database_Manager();

  void refresh();
private slots:
  void on_b_new_database_clicked();

  void on_b_open_saved_database_folder_clicked();

  void on_b_import_database_clicked();

private:
  Ui::W_Database_Manager* ui;
};

#endif // W_DATABASE_MANAGER_H
