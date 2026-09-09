#ifndef DB_SETTINGS_H
#define DB_SETTINGS_H

#include <QDateTime>
#include <QVariantMap>

enum class EResponsibility {
  Undefined,
  Tenant,
  Landlord,
  Thrid_Party,
  Both,
};

struct CurrencyInfo {
  QString isoCode;
  QString symbol;
  QString name;

  QString to_str() const
  {
    return name + " (" + symbol + " - " + isoCode + ")";
  }

  static CurrencyInfo from_iso(const QString& iso);
  static CurrencyInfo from_local_lang(const QString& currency_iso, QLocale::Language lang);
};

class DB_Settings
{
  using EResp = EResponsibility;

public:
  DB_Settings() = default;

  DB_Settings(const QString& database_path);

  // save operation
  void load();
  void save(const QString& _save_method);

  // getters
  [[nodiscard]] QDateTime get_save_time() const
  {
    return last_save_time;
  }
  [[nodiscard]] QString get_name() const
  {
    return name;
  }
  [[nodiscard]] QString get_description() const
  {
    return description;
  }
  [[nodiscard]] QString get_save_method() const
  {
    return save_method;
  }
  [[nodiscard]] CurrencyInfo get_currency() const
  {
    return currency;
  }
  [[nodiscard]] QLocale get_language() const
  {
    return language;
  }
  [[nodiscard]] QString get_uuid() const
  {
    return uuid;
  }

  [[nodiscard]] EResp get_household_waste_payer() const
  {
    return household_waste_payer;
  }
  [[nodiscard]] EResp get_housing_aid_payer() const
  {
    return housing_aid_payer;
  }
  [[nodiscard]] EResp get_charge_payer() const
  {
    return charge_payer;
  }
  [[nodiscard]] EResp get_charge_advance_payer() const
  {
    return charge_advance_payer;
  }
  [[nodiscard]] EResp get_water_consumption_payer() const
  {
    return water_consumption_payer;
  }
  [[nodiscard]] EResp get_water_subscription_payer() const
  {
    return water_subscription_payer;
  }
  [[nodiscard]] EResp get_charge_collected_payer() const
  {
    return charge_collected_payer;
  }

  [[nodiscard]] bool is_valid() const;

  // setters
  void set_save_time(const QDateTime& save_time)
  {
    last_save_time = save_time;
  }
  void set_description(const QString& _description)
  {
    description = _description;
  }
  void set_name(const QString& _name)
  {
    name = _name;
  }
  void set_language(const QLocale& _language)
  {
    language = _language;
  }
  void set_currency(const QString& _currency_iso)
  {
    currency = CurrencyInfo::from_iso(_currency_iso);
  }

  void set_household_waste_payer(EResp _new_resp)
  {
    household_waste_payer = _new_resp;
  }
  void set_housing_aid_payer(EResp _new_resp)
  {
    housing_aid_payer = _new_resp;
  }
  void set_charge_payer(EResp _new_resp)
  {
    charge_payer = _new_resp;
  }
  void set_charge_advance_payer(EResp _new_resp)
  {
    charge_advance_payer = _new_resp;
  }
  void set_water_consumption_payer(EResp _new_resp)
  {
    water_consumption_payer = _new_resp;
  }
  void set_water_subscription_payer(EResp _new_resp)
  {
    water_subscription_payer = _new_resp;
  }
  void set_charge_collected_payer(EResp _new_resp)
  {
    charge_collected_payer = _new_resp;
  }

private:
  QString db_settings_path;

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

#endif // DB_SETTINGS_H
