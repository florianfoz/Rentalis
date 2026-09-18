#include "widget/database/w_database_creator.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "database/w_database_manager.h"
#include "preferences.h"
#include "savemanager.h"
#include "ui_w_database_creator.h"

#include <QLocale>
#include <QTimer>

W_Database_Creator::W_Database_Creator(QWidget* parent, const QString& database_path)
  : QDialog(parent)
  , ui(new Ui::W_Database_Creator)
{
  ui->setupUi(this);

  if (!database_path.isEmpty()) {
    if (Database_Manager::current_database() && database_path == Database_Manager::current_database()->folder_path())
      database = *Database_Manager::current_database();
    else
      database = Database::load(database_path);
  }

  auto update_current_time = [this]() {
    QDateTime dt = QDateTime::currentDateTime();
    ui->le_datetime->setText(dt.toString("dd/MM/yyyy HH:mm:ss"));
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


  for (auto [iso, sym] : CURRENCIES.asKeyValueRange()) {
    ui->cb_currency->addItem(QString("%1 (%2)").arg(iso, sym), iso);
  }

  if (database) {
    int currency_index = ui->cb_currency->findData(database.manifest.currency.isoCode);
    ui->cb_currency->setCurrentIndex(currency_index);
    int language_index = ui->cb_language->findData(QLocale::languageToCode(database.manifest.language.language()));
    ui->cb_language->setCurrentIndex(language_index);
    ui->le_datetime->setText(database.manifest.last_save_time.toString("dd/MM/yyyy HH:mm:ss"));
    ui->le_name->setText(database.manifest.name);
    ui->te_db_description->setText(database.manifest.description);
  }
}

void W_Database_Creator::refresh()
{
}

void W_Database_Creator::clear()
{
  delete timer;
}

void W_Database_Creator::inject_data()
{
  if (database) {
    database.manifest.currency    = CurrencyInfo::from_iso(ui->cb_currency->currentData().toString());
    database.manifest.language    = QLocale::codeToLanguage(ui->cb_language->currentData().toString());
    database.manifest.name        = ui->le_name->text();
    database.manifest.description = ui->te_db_description->toPlainText();
  }
}


void W_Database_Creator::create()
{
  QString currency_iso = ui->cb_currency->currentData().toString();

  database = Database::create(sanitize_fileName(ui->le_name->text()), ui->te_db_description->document()->toMarkdown(),
                              currency_iso, Preferences::locale());

  is_creation_successful = database.is_valid();
}

void W_Database_Creator::remove_dialog_buttons()
{
  ui->buttonBox->close();
}


void W_Database_Creator::on_buttonBox_accepted()
{
  if (database) {
    inject_data();
    SaveManager::export_save(database.path, SAVE_PATH() + "/" + database.manifest.name);
    database.manifest.save("manifest edited");
  } else {
    create();
  }

  auto* parent = parentWidget();

  while (parent) {
    if (auto* manager = qobject_cast<W_Database_Manager*>(parent)) {
      manager->refresh();
      break;
    }

    parent = parent->parentWidget();
  }
}


void W_Database_Creator::on_buttonBox_rejected()
{
  close();
}
