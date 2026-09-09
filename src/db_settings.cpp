#include "db_settings.h"

#include <QFile>
#include <QSettings>
#include <QUuid>

DB_Settings::DB_Settings(const QString& db_folder_path)
{
  if (!QFile::exists(db_folder_path + "/config.ini")) {
    qCritical() << "database settings file at " << db_folder_path << "/config.ini dosen't exists !";
    return;
  }

  db_settings_path = db_folder_path + "/config.ini";

  load();
}

void DB_Settings::load()
{
  if (!is_valid()) return;

  QSettings settings(db_settings_path);

  uuid           = settings.value("database/uuid", QUuid::createUuid().toString(QUuid::WithoutBraces)).toString();
  name           = settings.value("database/name", "no_name").toString();
  last_save_time = settings.value("database/last_save_time", QDateTime::currentDateTime()).toDateTime();
  description    = settings.value("database/description", "").toString();
  save_method    = settings.value("database/save_method", "root").toString();
  currency       = CurrencyInfo::from_iso(settings.value("database/currency").toString());
  language       = QLocale(settings.value("database/language", QLocale::system().name()).toString());
}

bool DB_Settings::is_valid() const
{
  QSettings settings(db_settings_path);
  settings.setValue("test/key", 777);
  if (settings.value("test/key") != 777) {
    return false;
  }
  // clear test
  settings.remove("test/key");

  return true;
}

void DB_Settings::save(const QString& _save_method)
{
  if (!is_valid()) return;

  save_method = _save_method;

  QSettings settings(db_settings_path);

  settings.setValue("database/uuid", uuid);
  settings.setValue("database/name", name);
  settings.setValue("database/last_save_time", last_save_time);
  settings.setValue("database/description", description);
  settings.setValue("database/save_method", save_method);
  settings.setValue("database/currency", currency.isoCode);
  settings.setValue("database/language", language);
}


CurrencyInfo CurrencyInfo::from_iso(const QString& iso)
{
  const auto locales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);

  for (const QLocale& loc : locales) {
    if (loc.currencySymbol(QLocale::CurrencyIsoCode) == iso) {
      return CurrencyInfo{
          iso,
          loc.currencySymbol(QLocale::CurrencySymbol),
          loc.currencySymbol(QLocale::CurrencyDisplayName),
      };
    }
  }

  return CurrencyInfo{iso, iso, iso};
}

CurrencyInfo CurrencyInfo::from_local_lang(const QString& currency_iso, QLocale::Language lang)
{
  QList<QLocale> allLocales = QLocale::matchingLocales(lang, QLocale::AnyScript, QLocale::AnyCountry);

  for (const QLocale& locale : allLocales) {
    if (locale.currencySymbol(QLocale::CurrencyIsoCode) == currency_iso) {
      CurrencyInfo currency{
          locale.currencySymbol(QLocale::CurrencyIsoCode),
          locale.currencySymbol(QLocale::CurrencySymbol),
          locale.currencySymbol(QLocale::CurrencyDisplayName),
      };
      return currency;
    }
  }

  QList<QLocale> anyLocales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);
  for (const QLocale& locale : anyLocales) {
    if (locale.currencySymbol(QLocale::CurrencyIsoCode) == currency_iso) {
      CurrencyInfo currency{
          locale.currencySymbol(QLocale::CurrencyIsoCode),
          locale.currencySymbol(QLocale::CurrencySymbol),
          locale.currencySymbol(QLocale::CurrencyDisplayName),
      };
      return currency;
    }
  }

  return CurrencyInfo{currency_iso, currency_iso, currency_iso};
}
