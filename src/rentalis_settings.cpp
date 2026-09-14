#include "rentalis_settings.h"

#include "base.h"

#include <QDir>
#include <QFile>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

RentalisSettings::RentalisSettings()
{
  load();
}

void RentalisSettings::load()
{
  auto settings = QSettings("Rentalis", "Rentalis");
  send_email    = settings.value("database/send_email", "INVALID").toString();
  last_database = settings.value("software/last_database", "none").toString();
  language      = settings.value("software/language", QLocale::system().name()).toString();
  ask_ai_url    = settings.value("software/ask_ai_url", "https://your-domain.com/c/new?prompt=%1&submit=%2").toString();
  theme         = settings.value("software/theme", "ERROR").toString();
  custom_theme  = settings.value("software/custom_theme", THEME_LIGHT_CSS).toString();

  for (auto it = warning_invoice.begin(); it != warning_invoice.end(); ++it) {
    warning_invoice[it.key()] = settings.value("software/warning/invoice/" + it.key(), it.value());
  }


  warning_invoice = {
      {"invoice",                      true},
      {"water_price",                  true},
      {"water_price_threshold",        1.0F},
      {"water_subscription",           true},
      {"water_subscription_threshold", 1.0F},
      {"water_last_check",             true},
      {"water_last_check_threshold",   1.0F},
      {"water_check",                  true},
      {"water_check_threshold",        1.0F},
      {"dates",                        true},
  };

  warning_maintenance = {
      {"maintenance",    true},
      {"cost",           true},
      {"cost_threshold", 0.0F},
      {"dates",          true},
  };

  warning_receipt = {
      {"receipt",                  true},
      {"charge",                   true},
      {"charge_threshold",         0.0F},
      {"charge_advance",           true},
      {"charge_advance_threshold", 0.0F},
      {"dates",                    true},
  };

  warning_damage = {
      {"damage",        true},
      {"cost",          true},
      {"cost_treshold", 0.0F},
      {"date",          true},
  };

  warning_room = {
      {"room",                   true},
      {"surface_size",           true},
      {"surface_size_threshold", 9.0F},
  };

  warning_feature = {
      {"feature",                true},
      {"surface_size",           true},
      {"surface_size_threshold", 0.0F},
  };
}

void RentalisSettings::save()
{
  auto settings = QSettings("Rentalis", "Rentalis");
  settings.setValue("database/send_email", send_email);
  settings.setValue("software/language", language);
  settings.setValue("software/ask_ai_url", ask_ai_url);
  settings.setValue("software/theme", theme);
  settings.setValue("software/custom_theme", custom_theme);

  for (auto it = warning_invoice.begin(); it != warning_invoice.end(); ++it) {
    settings.setValue("software/warning/invoice/" + it.key(), it.value());
  }
}

QLocale RentalisSettings::locale()
{
  QString base_name          = "Rentalis_" + language;       // ex: "Rentalis_fr_FR"
  QString base_name_fallback = base_name.section('_', 0, 1); // ex: "Rentalis_fr"

  // check in resources
  QString software_t    = ":/translations/" + base_name + ".qm";
  QString software_fb_t = ":/translations/" + base_name_fallback + ".qm";

  if (QFile::exists(software_t)) return QLocale(software_t);
  if (QFile::exists(software_fb_t)) return QLocale(software_fb_t);

  // check in user folders
  QString user_t    = TRADUCTION_PATH() + "/user/" + base_name + ".qm";
  QString user_fb_t = TRADUCTION_PATH() + "/user/" + base_name_fallback + ".qm";

  if (QFile::exists(user_t)) return QLocale(user_t);
  if (QFile::exists(user_fb_t)) return QLocale(user_fb_t);

  // no file found
  return {};
}

QString RentalisSettings::theme_css()
{
  if (theme == "") return "";

  if (theme == "rentalis-standard") return "";
  if (theme == "rentalis-light") return THEME_LIGHT_CSS;
  if (theme == "rentalis-dark") return THEME_DARK_CSS;
  if (theme == "custom") return custom_theme;

  QStringList user_th = QDir(THEME_PATH() + "/user").entryList(QStringList() << "*.css", QDir::Files);
  for (const auto& t : user_th) {
    if (theme == t) {
      QFile file(QDir(THEME_PATH() + "/user").filePath(t));
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString     content = in.readAll();
        file.close();
        return content;
      }
    }
  }

  return "";
}

QVariant RentalisSettings::invoice_warning(const QString& param)
{
  if (auto it = warning_invoice.find(param); it != warning_invoice.end()) return it.value();

  qCritical() << "Invalid param name";
  return {};
}

QVariant RentalisSettings::maintenance_warning(const QString& param)
{
  if (auto it = warning_maintenance.find(param); it != warning_maintenance.end()) return it.value();

  qCritical() << "Invalid param name";
  return {};
}

QVariant RentalisSettings::receipt_warning(const QString& param)
{
  if (auto it = warning_receipt.find(param); it != warning_receipt.end()) return it.value();

  qCritical() << "Invalid param name";
  return {};
}

QVariant RentalisSettings::damage_warning(const QString& param)
{
  if (auto it = warning_damage.find(param); it != warning_damage.end()) return it.value();

  qCritical() << "Invalid param name";
  return {};
}

QVariant RentalisSettings::room_warning(const QString& param)
{
  if (auto it = warning_room.find(param); it != warning_room.end()) return it.value();
  qCritical() << "Invalid param name";
  return {};
}

QVariant RentalisSettings::feature_warning(const QString& param)
{
  if (auto it = warning_feature.find(param); it != warning_feature.end()) return it.value();
  qCritical() << "Invalid param name";
  return {};
}

void RentalisSettings::set_invoice_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_invoice.find(param); it != warning_invoice.end())
    it.value() = val;
  else
    qCritical() << "Invalid param name";
}

void RentalisSettings::set_maintenance_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_maintenance.find(param); it != warning_maintenance.end())
    it.value() = val;
  else
    qCritical() << "Invalid param name";
}

void RentalisSettings::set_receipt_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_receipt.find(param); it != warning_receipt.end())
    it.value() = val;
  else
    qCritical() << "Invalid param name";
}

void RentalisSettings::set_damage_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_damage.find(param); it != warning_damage.end())
    it.value() = val;
  else
    qCritical() << "Invalid param name";
}

void RentalisSettings::set_room_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_room.find(param); it != warning_room.end())
    it.value() = val;
  else
    qCritical() << "Invalid param name";
}

void RentalisSettings::set_feature_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_feature.find(param); it != warning_feature.end())
    it.value() = val;
  else
    qCritical() << "Invalid param name";
}
