#ifndef W_DATABASE_CREATOR_H
#define W_DATABASE_CREATOR_H


#include "database/database.h"

#include <QDialog>

class Database;
class QTimer;

namespace Ui
{
class W_Database_Creator;
}

class W_Database_Creator : public QDialog
{
  Q_OBJECT

public:
  explicit W_Database_Creator(QWidget* parent = nullptr, const QString& database_path = {});
  ~W_Database_Creator();

  Database database;

  bool is_creation_successful = false;

  QTimer* timer = nullptr;

  void populate_ui();
  void refresh();
  void clear();
  void inject_data();

  void create();

  // for preferences windows case
  void remove_dialog_buttons();

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::W_Database_Creator* ui;
};

#endif // W_DATABASE_CREATOR_H
