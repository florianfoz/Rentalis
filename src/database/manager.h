#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QObject>

class MainMenu;
class Database;

class Database_Manager : public QObject
{
  Q_OBJECT
public:
  explicit Database_Manager(QObject* parent = nullptr);

  static Database_Manager& instance()
  {
    static auto db = Database_Manager();
    return db;
  }

  static bool load_database(const QString& db_path);

  static Database* current_database()
  {
    return database;
  }
  static QSqlDatabase current_sql();
  static MainMenu*    mainmenu()
  {
    return menu;
  }

  static bool is_valid(bool is_silent = false);

  inline static Database* database;
  inline static MainMenu* menu;

signals:
  void signal_db_changed();
  void signal_db_updated();
};

#endif // DATABASE_MANAGER_H
