#ifndef INVOICE_H
#define INVOICE_H

#include <QDate>
#include <QString>


class Invoice
{
public:
  Invoice(int id, bool lazy = false)
    : id(id)
    , loaded(false)
  {
    if (!lazy) ensure_loaded();
  }

private:
  void ensure_loaded();

public:
  // no copy
  Invoice(const Invoice&)            = delete;
  Invoice& operator=(const Invoice&) = delete;
  Invoice(Invoice&&)                 = default;
  Invoice& operator=(Invoice&&)      = default;

  // getters
  [[nodiscard]] int get_id() const
  {
    return id;
  }
  [[nodiscard]] int get_tenant_id()
  {
    ensure_loaded();
    return tenant_id;
  }
  [[nodiscard]] QDate get_start_date()
  {
    ensure_loaded();
    return start_date;
  }
  [[nodiscard]] QDate get_end_date()
  {
    ensure_loaded();
    return end_date;
  }
  [[nodiscard]] int get_last_water_check()
  {
    ensure_loaded();
    return last_water_check;
  }
  [[nodiscard]] int get_water_check()
  {
    ensure_loaded();
    return water_check;
  }
  [[nodiscard]] float get_water_price()
  {
    ensure_loaded();
    return water_price;
  }
  [[nodiscard]] float get_water_subscription()
  {
    ensure_loaded();
    return water_subscription;
  }
  [[nodiscard]] float get_charge_collected()
  {
    ensure_loaded();
    return charge_collected;
  }
  [[nodiscard]] float get_other()
  {
    ensure_loaded();
    return other;
  }
  [[nodiscard]] QString get_comment()
  {
    ensure_loaded();
    return comment;
  }
  [[nodiscard]] int get_water_consum()
  {
    ensure_loaded();
    return get_water_check() - get_last_water_check();
  }
  [[nodiscard]] float get_water_consum_price()
  {
    ensure_loaded();
    return get_water_consum() * get_water_price();
  }
  // rest for tenant to pay
  [[nodiscard]] float get_rest()
  {
    ensure_loaded();
    return get_water_consum_price() + get_water_subscription() - charge_collected;
  }
  [[nodiscard]] bool is_rest_for_tenant()
  {
    return get_rest() >= 0;
  }

  [[nodiscard]] bool is_loaded() const
  {
    return loaded;
  }
  [[nodiscard]] bool is_dirty() const
  {
    return dirty;
  }

  // setters
  void set_tenant_id(int _tenant_id)
  {
    dirty     = true;
    tenant_id = _tenant_id;
  }
  void set_start_date(const QDate& _start_date)
  {
    dirty      = true;
    start_date = _start_date;
  }
  void set_end_date(const QDate& _end_date)
  {
    dirty    = true;
    end_date = _end_date;
  }
  void set_last_water_check(float _last_water_check)
  {
    dirty            = true;
    last_water_check = _last_water_check;
  }
  void set_water_check(float _water_check)
  {
    dirty       = true;
    water_check = _water_check;
  }
  void set_water_price(float _water_price)
  {
    dirty       = true;
    water_price = _water_price;
  }
  void set_water_subscription(float _water_subscription)
  {
    dirty              = true;
    water_subscription = _water_subscription;
  }
  void set_charge_collected(float _charge_collected)
  {
    dirty            = true;
    charge_collected = _charge_collected;
  }
  void set_other(float _other)
  {
    dirty = true;
    other = _other;
  }
  void set_comment(const QString& _comment)
  {
    dirty   = true;
    comment = _comment;
  }

  // database operations
  bool delete_record();
  int  insert_record();
  bool update_record();

private:
  bool loaded = false;
  bool dirty  = false;

  int id = -1;

  int     tenant_id = -1;
  QDate   start_date;
  QDate   end_date;
  int     last_water_check   = 0;
  int     water_check        = 0;
  float   water_price        = 0.0f;
  float   water_subscription = 0.0f;
  float   charge_collected   = 0.0f;
  float   other              = 0.0f;
  QString comment;
};

#endif // INVOICE_H
