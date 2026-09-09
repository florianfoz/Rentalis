#include "widget/w_database_creator.h"

#include "base.h"
#include "database.h"
#include "db_settings.h"
#include "settings.h"
#include "ui_w_database_creator.h"

#include <QLocale>
#include <QTimer>

W_Database_Creator::W_Database_Creator(Save_Manager* manager, const QString& database_path)
  : ui(new Ui::W_Database_Creator)
  , manager(manager)
  , database(new Database())
{
  ui->setupUi(this);

  database->open_database(database_path);


  populate_ui();
}

W_Database_Creator::~W_Database_Creator()
{
  delete ui;
}

void W_Database_Creator::populate_ui()
{
  clear();

  ui->cb_currency->clear();

  QSet<QLocale::Language> lang_added;
  QSet<QString>           currency_added;

  // languages
  for (int i = 0; i < QLocale::Language::LastLanguage; ++i) {
    QLocale::Language lang = static_cast<QLocale::Language>(i);

    if (lang == QLocale::C || lang_added.contains(lang)) continue;

    ui->cb_language->addItem(QLocale::languageToString(lang), lang);

    lang_added.insert(lang);
  }

  // currencies
  const auto locales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);

  for (const QLocale& locale : locales) {
    QString code = locale.currencySymbol(QLocale::CurrencyIsoCode);

    if (code.isEmpty() || currency_added.contains(code)) continue;

    QString name   = locale.currencySymbol(QLocale::CurrencyDisplayName);
    QString symbol = locale.currencySymbol(QLocale::CurrencySymbol);

    ui->cb_currency->addItem(QString("%1 (%2 – %3)").arg(name, symbol, code), code);

    currency_added.insert(code);
  }

  if (database && database->is_valid()) {
    int currency_index = ui->cb_currency->findData(database->get_settings().get_currency().isoCode);
    ui->cb_currency->setCurrentIndex(currency_index);
    int language_index = ui->cb_language->findData(database->get_settings().get_language());
    ui->cb_language->setCurrentIndex(language_index);
    ui->le_datetime->setText(database->get_settings().get_save_time().toString("dd/MM/yyyy HH:mm:ss"));
    ui->le_name->setText(database->get_settings().get_name());
    ui->te_db_description->setText(database->get_settings().get_description());
  } else {
    auto update_current_time = [this]() {
      ui->le_datetime->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss"));
    };

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, update_current_time);

    timer->start(1000);
    update_current_time();
  }
}

void W_Database_Creator::update_ui()
{
}

void W_Database_Creator::clear()
{
  if (timer) delete timer;
}

void W_Database_Creator::inject_data()
{
  if (!database) return;
  database->get_settings().set_currency(ui->cb_currency->currentData().toString());
  database->get_settings().set_description(ui->te_db_description->document()->toMarkdown());
  QLocale lang(static_cast<QLocale::Language>(ui->cb_language->currentData().toInt()));
  database->get_settings().set_language(lang);
  database->get_settings().set_name(ui->le_name->text());
  is_creation_successful = database->update_metadata();
}


void W_Database_Creator::create()
{
  QString currency_iso = ui->cb_currency->currentData().toString();

  Database database;
  database.new_database(ui->le_name->text(), ui->te_db_description->document()->toMarkdown(), currency_iso,
                        SETTINGS.get_locale());
}


void W_Database_Creator::on_buttonBox_accepted()
{
  inject_data();
}


void W_Database_Creator::on_buttonBox_rejected()
{
  close();
}
