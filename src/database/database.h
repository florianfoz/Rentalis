#ifndef DATABASE_H
#define DATABASE_H

#include "database/manifest.h"
#include "database/recorder.h"
#include "forward.h"

#include <QDate>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>


const QMap<ETable, QString> DB_SCHEMA = {
    {ETable::Property, {}}
};


class Database
{
public:
  [[nodiscard]] static Database create(const QString& _name, const QString& _description, const QString& _currency_iso,
                                       const QLocale& _language);
  [[nodiscard]] static Database load(const QString& path);
  void                          unload();

  [[nodiscard]] static Database& invalid()
  {
    static auto db    = Database();
    db.database_valid = false;
    return db;
  }

  // getters
  [[nodiscard]] QString file_name() const;

  [[nodiscard]] bool is_valid() const
  {
    return database_valid;
  }

  explicit operator bool() const
  {
    return is_valid();
  }

  [[nodiscard]] QString folder_name() const;
  [[nodiscard]] QString folder_path() const;
  [[nodiscard]] QString file_path() const;
  [[nodiscard]] QString manifest_path() const;

  [[nodiscard]] bool check_db_integrity() const;

  Database_Manifest manifest;
  Database_Recorder recorder;
  bool              database_valid = false;
  QString           path;
};

#endif // DATABASE_H
