#include "settings.h"

#include "base.h"

#include <QDir>
#include <QFile>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

Settings::Settings()
{
  load();
}

void Settings::load()
{
  auto settings = QSettings("Rentalis", "Rentalis");
  send_email    = settings.value("database/send_email", "INVALID").toString();
  last_db       = settings.value("software/last_database", "none").toString();
  language      = settings.value("software/language", QLocale::system().name()).toString();
  ask_ai_url    = settings.value("software/ask_ai_url", "https://your-domain.com/c/new?prompt=%1&submit=%2").toString();
  theme         = settings.value("software/theme", "ERROR").toString();
  custom_theme  = settings.value("software/custom_theme", THEME_LIGHT_CSS).toString();

  for (auto it = warning_invoice.begin(); it != warning_invoice.end(); ++it) {
    warning_invoice[it.key()] = settings.value("software/warning/invoice/" + it.key(), it.value());
  }
}

void Settings::save()
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

QLocale Settings::get_locale() const
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

  if (QFile::exists(user_t))
    return QLocale(user_t);
  else if (QFile::exists(user_fb_t))
    return QLocale(user_fb_t);

  // no file found
  return QLocale();
}

QString Settings::get_theme_css() const
{
  if (theme == "") return "";

  if (theme == "rentalis-standard")
    return "";
  else if (theme == "rentalis-light")
    return THEME_LIGHT_CSS;
  else if (theme == "rentalis-dark")
    return THEME_DARK_CSS;
  else if (theme == "custom")
    return custom_theme;
  else {
    QStringList user_th = QDir(THEME_PATH() + "/user").entryList(QStringList() << "*.css", QDir::Files);
    for (const QString& t : user_th) {
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
  }

  return "";
}

QString Settings::get_theme() const
{
  return theme;
}

QVariant Settings::get_invoice_warning(const QString& param)
{
  if (auto it = warning_invoice.find(param); it != warning_invoice.end()) {
    return it.value();
  } else {
    qCritical() << "Invalid param name";
    return QVariant{};
  }
}

QVariant Settings::get_maintenance_warning(const QString& param)
{
  if (auto it = warning_maintenance.find(param); it != warning_maintenance.end()) {
    return it.value();
  } else {
    qCritical() << "Invalid param name";
    return QVariant{};
  }
}

QVariant Settings::get_receipt_warning(const QString& param)
{
  if (auto it = warning_receipt.find(param); it != warning_receipt.end()) {
    return it.value();
  } else {
    qCritical() << "Invalid param name";
    return QVariant{};
  }
}

QVariant Settings::get_damage_warning(const QString& param)
{
  if (auto it = warning_damage.find(param); it != warning_damage.end()) {
    return it.value();
  } else {
    qCritical() << "Invalid param name";
    return QVariant{};
  }
}

QVariant Settings::get_room_warning(const QString& param)
{
  if (auto it = warning_room.find(param); it != warning_room.end()) {
    return it.value();
  } else {
    qCritical() << "Invalid param name";
    return QVariant{};
  }
}

QVariant Settings::get_feature_warning(const QString& param)
{
  if (auto it = warning_feature.find(param); it != warning_feature.end()) {
    return it.value();
  } else {
    qCritical() << "Invalid param name";
    return QVariant{};
  }
}

void Settings::set_invoice_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_invoice.find(param); it != warning_invoice.end()) {
    it.value() = val;
  } else {
    qCritical() << "Invalid param name";
  }
}

void Settings::set_maintenance_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_maintenance.find(param); it != warning_maintenance.end()) {
    it.value() = val;
  } else {
    qCritical() << "Invalid param name";
  }
}

void Settings::set_receipt_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_receipt.find(param); it != warning_receipt.end()) {
    it.value() = val;
  } else {
    qCritical() << "Invalid param name";
  }
}

void Settings::set_damage_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_damage.find(param); it != warning_damage.end()) {
    it.value() = val;
  } else {
    qCritical() << "Invalid param name";
  }
}

void Settings::set_room_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_room.find(param); it != warning_room.end()) {
    it.value() = val;
  } else {
    qCritical() << "Invalid param name";
  }
}

void Settings::set_feature_warning(const QString& param, const QVariant& val)
{
  if (auto it = warning_feature.find(param); it != warning_feature.end()) {
    it.value() = val;
  } else {
    qCritical() << "Invalid param name";
  }
}
