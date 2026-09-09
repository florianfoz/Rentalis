#include "widget/w_welcome.h"

#include "base.h"
#include "settings.h"
#include "ui_w_welcome.h"
#include "widget/w_database_choice.h"

#include <QDir>
#include <QLocale>

W_Welcome::W_Welcome(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::W_Welcome)
{
  ui->setupUi(this);

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
}

W_Welcome::~W_Welcome()
{
  QString lang_iso = ui->cb_language->currentData().toString();
  SETTINGS.set_language(lang_iso);

  delete ui;
}

void W_Welcome::on_b_next_clicked()
{
  auto w_db_choice = new W_Database_Choice();
  w_db_choice->setModal(true);
  w_db_choice->exec();
}
