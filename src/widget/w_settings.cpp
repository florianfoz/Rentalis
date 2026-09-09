#include "widget/w_settings.h"

#include "base.h"
#include "csshighlighter.h"
#include "settings.h"
#include "ui_w_settings.h"
#include "widget/w_database_creator.h"

#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>
#include <QStandardPaths>

W_Settings::W_Settings(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Settings)
{
  ui->setupUi(this);

  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");

  high = new CssHighlighter(ui->te_custom_theme_CSS->document());

  SETTINGS.load();

  ui->cb_language->clear();
  ui->cb_theme->clear();

  QStringList software_ts = QDir(":/translations/").entryList(QDir::Files);
  for (auto& t : software_ts) {
    QString loc = getLocaleFromFile(t);
    QLocale iso(loc);

    ui->cb_language->addItem(iso.nativeLanguageName() + ", " + iso.nativeTerritoryName(), iso.name());
  }

  QStringList user_ts = QDir(TRADUCTION_PATH() + "/user").entryList(QStringList() << "*.qm", QDir::Files);
  for (auto& t : user_ts) {
    QString loc = getLocaleFromFile(t);
    QLocale iso(loc);

    ui->cb_language->addItem(iso.nativeLanguageName() + ", " + iso.nativeTerritoryName(), iso.name());
  }

  int lang_index = ui->cb_language->findData(SETTINGS.get_locale().name());
  ui->cb_language->setCurrentIndex(lang_index);

  ui->cb_theme->addItem("Rentalis - Standard", "rentalis-standard");
  ui->cb_theme->addItem("Rentalis - Light", "rentalis-light");
  ui->cb_theme->addItem("Rentalis - Dark", "rentalis-dark");
  ui->cb_theme->addItem("Custom", "custom");

  QStringList user_th = QDir(THEME_PATH() + "/user").entryList(QStringList() << "*.css", QDir::Files);
  for (const QString& t : user_th) {
    ui->cb_theme->addItem("user/" + t, t);
  }

  int theme_index = ui->cb_theme->findData(SETTINGS.get_theme());
  ui->cb_theme->setCurrentIndex(theme_index);

  ui->te_custom_theme_CSS->setText(SETTINGS.get_custom_theme());
}

W_Settings::~W_Settings()
{
  delete ui;
}

void W_Settings::update_settings()
{
  SETTINGS.set_ask_ai_url(ui->le_ask_ai_url->text());
  QString lang = ui->cb_language->currentData().toString();
  SETTINGS.set_language(lang);
  QString th = ui->cb_theme->currentData().toString();
  SETTINGS.set_theme(th);
  SETTINGS.set_custom_theme(ui->te_custom_theme_CSS->toPlainText());

  APP->setStyleSheet(SETTINGS.get_theme_css());

  SETTINGS.save();
}


void W_Settings::on_buttonBox_accepted()
{
  update_settings();

  close();
}


void W_Settings::on_buttonBox_rejected()
{
  close();
}


void W_Settings::on_b_ask_to_ai_url_help_clicked()
{
  QString msg =
      tr("<b>Usage Instructions for AI Prompt URL Field</b><br>"
         "In this field, you can enter a URL template for generating prompts with an AI service. Use the placeholders "
         "below to customize the URL:"
         "<ul>"
         "<li><b>%1</b> – This placeholder will be replaced by the actual prompt text you want to send.</li>"
         "<li><b>%2</b> – This placeholder can be used to set the value of the <code>submit</code> parameter if the AI "
         "service supports automatic submission.</li>"
         "</ul>"
         "Example URL template:"
         "<pre>https://your-ai-instance.com/c/new?prompt=%1&submit=%2</pre>"
         "This template allows you to quickly generate URLs for testing or automating prompt submissions while keeping "
         "the prompt dynamic.");

  QMessageBox::information(this, tr("How to use a AI Prompt URL"), msg);
}


void W_Settings::on_b_reset_theme_clicked()
{
  ui->te_custom_theme_CSS->setText(THEME_LIGHT_CSS);
  APP->setStyleSheet(THEME_LIGHT_CSS);
}


void W_Settings::on_buttonBox_clicked(QAbstractButton* button)
{
  if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole) {
    update_settings();
  } else if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole) {
    update_settings();
    close();
  } else if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole) {
    close();
  }
}


void W_Settings::on_cb_theme_currentIndexChanged(int index)
{
  ui->gb_custom_theme->setHidden(ui->cb_theme->currentData().toString() != "custom");
}


void W_Settings::on_b_theme_folder_clicked()
{
  auto f = QFileInfo(THEME_PATH());
  QDesktopServices::openUrl(QUrl::fromLocalFile(f.absolutePath()));
}


void W_Settings::on_b_db_settings_clicked()
{
  auto w_db_settings = new W_Database_Creator(nullptr, DATABASE_PATH());
  w_db_settings->setWindowTitle("Database Settings");
  w_db_settings->setModal(false);
  w_db_settings->exec();
}
