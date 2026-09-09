#include "widget/w_database_current_info.h"

#include "base.h"
#include "ui_w_database_current_info.h"

W_Database_Current_Info::W_Database_Current_Info(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Database_Current_Info)
{
  ui->setupUi(this);

  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

W_Database_Current_Info::~W_Database_Current_Info()
{
  delete ui;
}

void W_Database_Current_Info::refresh()
{
  if (!DB_MANAGER.is_valid()) return;

  QLocale      lang     = DB_MANAGER.get_db()->get_settings().get_language();
  CurrencyInfo currency = DB_MANAGER.get_db()->get_settings().get_currency();

  ui->le_currency->setText(currency.to_str());
  ui->le_datetime->setText(DB_MANAGER.get_db()->get_settings().get_save_time().toString("dd/MM/yyyy HH:mm:ss"));
  ui->le_language->setText(lang.nativeLanguageName());
  ui->le_name->setText(DB_MANAGER.get_db()->get_settings().get_name());
}
