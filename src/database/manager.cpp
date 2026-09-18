#include "database/manager.h"

#include "base.h"
#include "database/database.h"
#include "database/manifest.h"
#include "database/w_database_creator.h"
#include "database/w_database_manager.h"
#include "preferences.h"
#include "widget/w_mainmenu.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>

Database_Manager::Database_Manager(QObject* parent)
  : QObject{parent}
{
}


QSqlDatabase Database_Manager::current_sql()
{
  if (database) return database->recorder.sql;
  return {};
}

Database_Manifest* Database_Manager::current_manifest()
{
  if (database) return &database->manifest;
  return nullptr;
}

Database_Recorder* Database_Manager::current_recorder()
{
  if (database) return &database->recorder;
  return nullptr;
}
