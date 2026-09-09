#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "database.h"

#include <QObject>

class MainMenu;

class Database_Manager : public QObject
{
  Q_OBJECT
public:
  explicit Database_Manager(QObject* parent = nullptr);

  void set_mainmenu(MainMenu* _menu)
  {
    menu = _menu;
  }
  bool change_database(std::unique_ptr<Database> new_db);

  Database* get_db()
  {
    return db.get();
  }
  QSqlDatabase get_sql_db();
  MainMenu*    get_mainmenu()
  {
    return menu;
  }

  bool is_valid(bool is_silent = false);

private:
  std::unique_ptr<Database> db;
  MainMenu*                 menu;

signals:
  void signal_db_changed();
  void signal_db_updated();
};

#endif // DATABASE_MANAGER_H
