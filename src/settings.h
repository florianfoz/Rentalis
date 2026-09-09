#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFile>
#include <QString>
#include <QVariantMap>

class Settings
{
public:
  Settings();

  void load();
  void save();

  bool get_general_dates() const
  {
    return general_date;
  }
  QLocale get_locale() const;
  QString get_theme_css() const;
  QString get_theme() const;
  QString get_ask_ai_url() const
  {
    return ask_ai_url;
  }
  QString get_custom_theme() const
  {
    return custom_theme;
  }
  QString get_send_email() const
  {
    return send_email;
  }
  QString get_last_db() const
  {
    return last_db;
  }
  QVariant get_invoice_warning(const QString& param);
  QVariant get_maintenance_warning(const QString& param);
  QVariant get_receipt_warning(const QString& param);
  QVariant get_damage_warning(const QString& param);
  QVariant get_room_warning(const QString& param);
  QVariant get_feature_warning(const QString& param);

  void set_general_dates(bool _dates)
  {
    general_date = _dates;
  }
  void set_send_email(const QString& _send_email)
  {
    send_email = _send_email;
  }
  void set_language(const QString& _language)
  {
    language = _language;
  }
  void set_ask_ai_url(const QString& _ask_ai_url)
  {
    ask_ai_url = _ask_ai_url;
  }
  void set_theme(const QString& _theme)
  {
    theme = _theme;
  }
  void set_custom_theme(const QString& _custom_theme)
  {
    custom_theme = _custom_theme;
  }
  void set_all_warnings(bool _warning)
  {
    all_warnings = _warning;
  }
  void set_last_db(const QString& _last_db)
  {
    last_db = _last_db;
  }
  void set_invoice_warning(const QString& param, const QVariant& val);
  void set_maintenance_warning(const QString& param, const QVariant& val);
  void set_receipt_warning(const QString& param, const QVariant& val);
  void set_damage_warning(const QString& param, const QVariant& val);
  void set_room_warning(const QString& param, const QVariant& val);
  void set_feature_warning(const QString& param, const QVariant& val);

private:
  QString send_email;
  QString language;
  QString ask_ai_url;
  QString theme;
  QString custom_theme;
  QString last_db;

  bool general_date;

  bool all_warnings;

  QVariantMap warning_invoice = {
      {"invoice",                      true},
      {"water_price",                  true},
      {"water_price_threshold",        1.0f},
      {"water_subscription",           true},
      {"water_subscription_threshold", 1.0f},
      {"water_last_check",             true},
      {"water_last_check_threshold",   1.0f},
      {"water_check",                  true},
      {"water_check_threshold",        1.0f},
      {"dates",                        true},
  };

  QVariantMap warning_maintenance = {
      {"maintenance",    true},
      {"cost",           true},
      {"cost_threshold", 0.0f},
      {"dates",          true},
  };

  QVariantMap warning_receipt = {
      {"receipt",                  true},
      {"charge",                   true},
      {"charge_threshold",         0.0f},
      {"charge_advance",           true},
      {"charge_advance_threshold", 0.0f},
      {"dates",                    true},
  };

  QVariantMap warning_damage = {
      {"damage",        true},
      {"cost",          true},
      {"cost_treshold", 0.0f},
      {"date",          true},
  };

  QVariantMap warning_room = {
      {"room",                   true},
      {"surface_size",           true},
      {"surface_size_threshold", 9.0f},
  };

  QVariantMap warning_feature = {
      {"feature",                true},
      {"surface_size",           true},
      {"surface_size_threshold", 0.0f},
  };
};

#endif // SETTINGS_H
