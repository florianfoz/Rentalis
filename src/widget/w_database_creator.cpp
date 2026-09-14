#include "widget/w_database_creator.h"

#include "base.h"
#include "database/database.h"
#include "database/manifest.h"
#include "rentalis_settings.h"
#include "ui_w_database_creator.h"

#include <QLocale>
#include <QTimer>

W_Database_Creator::W_Database_Creator(W_Database_Manager* manager, const QString& database_path)
  : ui(new Ui::W_Database_Creator)
  , manager(manager)
  , database()
{
  ui->setupUi(this);

  if (!database_path.isEmpty()) database = Database::open_database(database_path);


  auto update_current_time = [this]() {
    QDate date = QDate::currentDate();
    ui->le_datetime->setText(date.toString("dd/MM/yyyy HH:mm:ss"));
  };

  auto* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, update_current_time);

  timer->start(1000);
  update_current_time();

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
  ui->cb_language->clear();

  // languages
  for (const auto& iso : LANGUAGES) {
    auto lang = QLocale(iso);

    ui->cb_language->addItem(QLocale::languageToString(lang.language()), iso);
  }


  for (const auto& iso : CURRENCIES) {
    const auto& sym = CURRENCIES[iso];

    ui->cb_currency->addItem(QString("%1 (%2)").arg(iso, sym), iso);
  }

  if (database) {
    int currency_index = ui->cb_currency->findData(database->manifest().currency.isoCode);
    ui->cb_currency->setCurrentIndex(currency_index);
    int language_index = ui->cb_language->findData(database->manifest().language);
    ui->cb_language->setCurrentIndex(language_index);
    ui->le_datetime->setText(database->manifest().last_save_time.toString("dd/MM/yyyy HH:mm:ss"));
    ui->le_name->setText(database->manifest().name);
    ui->te_db_description->setText(database->manifest().description);
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
  database->manifest().currency    = CurrencyInfo::from_iso(ui->cb_currency->currentData().toString());
  database->manifest().description = ui->te_db_description->document()->toMarkdown();
  QLocale lang(static_cast<QLocale::Language>(ui->cb_language->currentData().toInt()));
  database->manifest().language = lang;
  database->manifest().name     = ui->le_name->text();
  is_creation_successful        = database->update_metadata();
}


void W_Database_Creator::create()
{
  QString currency_iso = ui->cb_currency->currentData().toString();

  database = Database::new_database(ui->le_name->text(), ui->te_db_description->document()->toMarkdown(), currency_iso,
                                    RentalisSettings::locale());

  is_creation_successful = database->is_valid();
}


void W_Database_Creator::on_buttonBox_accepted()
{
  create();
}


void W_Database_Creator::on_buttonBox_rejected()
{
  close();
}
