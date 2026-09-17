#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "forward.h"

#include <QObject>

class Database;
class Database_Manifest;
class Database_Recorder;


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

  static Database* current_database()
  {
    return database;
  }
  static QSqlDatabase       current_sql();
  static Database_Manifest* current_manifest();
  static Database_Recorder* current_recorder();

  inline static Database* database = nullptr;

signals:
  void signal_db_changed();
  void signal_db_updated(ETable table);
};

#endif // DATABASE_MANAGER_H
