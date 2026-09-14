#ifndef RENTALIS_SETTINGS_H
#define RENTALIS_SETTINGS_H

#include <QFile>
#include <QString>
#include <QVariantMap>

class RentalisSettings
{
public:
  RentalisSettings();

  static void load();
  static void save();

  [[nodiscard]] static QLocale  locale();
  [[nodiscard]] static QString  theme_css();
  [[nodiscard]] static QVariant invoice_warning(const QString& param);
  [[nodiscard]] static QVariant maintenance_warning(const QString& param);
  [[nodiscard]] static QVariant receipt_warning(const QString& param);
  [[nodiscard]] static QVariant damage_warning(const QString& param);
  [[nodiscard]] static QVariant room_warning(const QString& param);
  [[nodiscard]] static QVariant feature_warning(const QString& param);

  static void set_invoice_warning(const QString& param, const QVariant& val);
  static void set_maintenance_warning(const QString& param, const QVariant& val);
  static void set_receipt_warning(const QString& param, const QVariant& val);
  static void set_damage_warning(const QString& param, const QVariant& val);
  static void set_room_warning(const QString& param, const QVariant& val);
  static void set_feature_warning(const QString& param, const QVariant& val);

  inline static QString send_email;
  inline static QString language;
  inline static QString ask_ai_url;
  inline static QString theme;
  inline static QString custom_theme;
  inline static QString last_database;
  inline static bool    general_date;
  inline static bool    all_warnings;

  inline static QVariantMap warning_invoice;
  inline static QVariantMap warning_maintenance;
  inline static QVariantMap warning_receipt;
  inline static QVariantMap warning_damage;
  inline static QVariantMap warning_room;
  inline static QVariantMap warning_feature;
};


#endif // RENTALIS_RENTALIS_SETTINGS_H
