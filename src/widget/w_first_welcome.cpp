#include "widget/w_first_welcome.h"

#include "base.h"
#include "preferences.h"
#include "ui_w_first_welcome.h"
#include "w_welcome.h"

#include <QDir>
#include <QLocale>

W_First_Welcome::W_First_Welcome(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::W_First_Welcome)
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

  int lang_index = ui->cb_language->findData(Preferences::locale().name());
  ui->cb_language->setCurrentIndex(lang_index);
}

W_First_Welcome::~W_First_Welcome()
{
  QString lang_iso      = ui->cb_language->currentData().toString();
  Preferences::language = lang_iso;

  delete ui;
}

void W_First_Welcome::on_b_next_clicked()
{
  close();
}
