#include "widget/database/w_database_current_info.h"

#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "ui_w_database_current_info.h"

W_Database_Current_Info::W_Database_Current_Info(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Database_Current_Info)
{
  ui->setupUi(this);

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable) { refresh(); });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

W_Database_Current_Info::~W_Database_Current_Info()
{
  delete ui;
}

void W_Database_Current_Info::refresh()
{
  if (!Database_Manager::current_database()) return;

  QLocale      lang     = Database_Manager::current_manifest()->language;
  CurrencyInfo currency = Database_Manager::current_manifest()->currency;

  ui->le_currency->setText(currency.to_str());
  ui->le_datetime->setText(Database_Manager::current_manifest()->last_save_time.toString("dd/MM/yyyy HH:mm:ss"));
  ui->le_language->setText(lang.nativeLanguageName());
  ui->le_name->setText(Database_Manager::current_manifest()->name);
}
