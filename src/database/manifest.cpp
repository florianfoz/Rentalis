#include "database/manifest.h"

#include <QFile>
#include <QSettings>
#include <QUuid>

Database_Manifest* Database_Manifest::create(const QString& config_path, const QString& _name, const QString& _desc)
{
  auto* out = new Database_Manifest();
  out->path = config_path;

  out->uuid           = QUuid::createUuid().toString(QUuid::WithoutBraces);
  out->name           = _name;
  out->last_save_time = QDateTime::currentDateTime();
  out->description    = _desc;
  out->save_method    = "db_creation";
  out->currency       = CurrencyInfo::from_iso("USD");
  out->language       = QLocale::system();

  out->save("db_creation");

  return out;
}

Database_Manifest* Database_Manifest::load(const QString& config_path)
{
  if (!QFile::exists(config_path)) {
    qCritical() << "database settings file at " << config_path << " dosen't exists !";
    return nullptr;
  }

  auto* out = new Database_Manifest();
  out->path = config_path;

  auto q_settings = QSettings(out->path, QSettings::IniFormat);

  out->uuid = q_settings.value("database/uuid", QUuid::createUuid().toString(QUuid::WithoutBraces)).toString();
  out->name = q_settings.value("database/name", "no_name").toString();
  out->last_save_time = q_settings.value("database/last_save_time", QDateTime::currentDateTime()).toDateTime();
  out->description    = q_settings.value("database/description", "").toString();
  out->save_method    = q_settings.value("database/save_method", "root").toString();
  out->currency       = CurrencyInfo::from_iso(q_settings.value("database/currency").toString());
  out->language       = QLocale(q_settings.value("database/language", QLocale::system().name()).toString());

  return out;
}

bool Database_Manifest::is_valid() const
{
  if (path.isEmpty()) return false;

  QSettings settings(path, QSettings::IniFormat);
  settings.setValue("test/key", 777);
  if (settings.value("test/key") != 777) {
    return false;
  }
  // clear test
  settings.remove("test/key");

  return true;
}

void Database_Manifest::save(const QString& _save_method)
{
  if (!is_valid()) return;

  save_method = _save_method;

  QSettings settings(path, QSettings::IniFormat);

  settings.setValue("database/uuid", uuid);
  settings.setValue("database/name", name);
  settings.setValue("database/last_save_time", last_save_time.toString(Qt::ISODate));
  settings.setValue("database/description", description);
  settings.setValue("database/save_method", save_method);
  settings.setValue("database/currency", currency.isoCode);
  settings.setValue("database/language", language.name());
}


CurrencyInfo CurrencyInfo::from_iso(const QString& iso)
{
  const auto locales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);

  for (const QLocale& loc : locales) {
    if (loc.currencySymbol(QLocale::CurrencyIsoCode) == iso) {
      return CurrencyInfo{
          .isoCode = iso,
          .symbol  = loc.currencySymbol(QLocale::CurrencySymbol),
          .name    = loc.currencySymbol(QLocale::CurrencyDisplayName),
      };
    }
  }

  return CurrencyInfo{.isoCode = iso, .symbol = iso, .name = iso};
}

CurrencyInfo CurrencyInfo::from_local_lang(const QString& currency_iso, QLocale::Language lang)
{
  QList<QLocale> allLocales = QLocale::matchingLocales(lang, QLocale::AnyScript, QLocale::AnyCountry);

  for (const QLocale& locale : allLocales) {
    if (locale.currencySymbol(QLocale::CurrencyIsoCode) == currency_iso) {
      CurrencyInfo currency{
          .isoCode = locale.currencySymbol(QLocale::CurrencyIsoCode),
          .symbol  = locale.currencySymbol(QLocale::CurrencySymbol),
          .name    = locale.currencySymbol(QLocale::CurrencyDisplayName),
      };
      return currency;
    }
  }

  QList<QLocale> anyLocales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);
  for (const QLocale& locale : anyLocales) {
    if (locale.currencySymbol(QLocale::CurrencyIsoCode) == currency_iso) {
      CurrencyInfo currency{
          .isoCode = locale.currencySymbol(QLocale::CurrencyIsoCode),
          .symbol  = locale.currencySymbol(QLocale::CurrencySymbol),
          .name    = locale.currencySymbol(QLocale::CurrencyDisplayName),
      };
      return currency;
    }
  }

  return CurrencyInfo{.isoCode = currency_iso, .symbol = currency_iso, .name = currency_iso};
}
