#ifndef DATABASE_MANIFEST_H
#define DATABASE_MANIFEST_H

#include "enum_lite.h"

#include <QDateTime>
#include <QVariantMap>

DEFINE_ENUM(EResponsibility, uint8_t, //
            Undefined, 1,             //
            Tenant, 2,                //
            Landlord, 3,              //
            Thrid_Party, 4,           //
            Both, 5,                  //
)

struct CurrencyInfo {
  QString isoCode;
  QString symbol;
  QString name;

  [[nodiscard]] QString to_str() const
  {
    return name + " (" + symbol + " - " + isoCode + ")";
  }

  static CurrencyInfo from_iso(const QString& iso);
  static CurrencyInfo from_local_lang(const QString& currency_iso, QLocale::Language lang);
};

class Database_Manifest
{
  using EResp = EResponsibility;

public:
  Database_Manifest() = default;

  // save operation
  [[nodiscard]] static Database_Manifest* create(const QString& config_path, const QString& _name,
                                                 const QString& _desc);
  [[nodiscard]] static Database_Manifest* load(const QString& config_path);
  void                                    save(const QString& _save_method);

  [[nodiscard]] bool is_valid() const;

  explicit operator bool() const noexcept
  {
    return is_valid();
  }

  QString path;

  QString      uuid;
  QString      name;
  QDateTime    last_save_time;
  QString      description;
  QString      save_method;
  CurrencyInfo currency;
  QLocale      language;

  EResp household_waste_payer    = EResp::Landlord;
  EResp housing_aid_payer        = EResp::Thrid_Party;
  EResp charge_payer             = EResp::Tenant;
  EResp charge_advance_payer     = EResp::Tenant;
  EResp water_consumption_payer  = EResp::Tenant;
  EResp water_subscription_payer = EResp::Landlord;
  EResp charge_collected_payer   = EResp::Tenant;
};


#endif // DATABASE_MANIFEST_H
