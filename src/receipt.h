#ifndef RECEIPT_H
#define RECEIPT_H

#include <QDate>
#include <QSqlQuery>
#include <QString>

enum class EDue;

class Receipt
{
public:
  Receipt(int id, bool lazy = false)
    : id(id)
    , loaded(false)
  {
    if (!lazy) ensure_loaded();
  }

  // no copy
  Receipt(const Receipt&)            = delete;
  Receipt& operator=(const Receipt&) = delete;
  Receipt(Receipt&&)                 = default;
  Receipt& operator=(Receipt&&)      = default;

private:
  void ensure_loaded() const;

public:
  // getters
  [[nodiscard]] int get_id() const
  {
    ensure_loaded();
    return id;
  }
  [[nodiscard]] int get_tenant_id() const
  {
    ensure_loaded();
    return tenant_id;
  }
  [[nodiscard]] QDate get_start_date() const
  {
    ensure_loaded();
    return start_date;
  }
  [[nodiscard]] QDate get_end_date() const
  {
    ensure_loaded();
    return end_date;
  }
  [[nodiscard]] float get_charge_advance() const
  {
    ensure_loaded();
    return charge_advance;
  }
  [[nodiscard]] EDue get_due() const
  {
    ensure_loaded();
    return due_id;
  }
  [[nodiscard]] QDate get_pay_date() const
  {
    ensure_loaded();
    return pay_date;
  }
  [[nodiscard]] QString get_comment() const
  {
    ensure_loaded();
    return comment;
  }
  [[nodiscard]] float get_total_receivable() const;
  [[nodiscard]] float get_tenant_receivable() const;

  [[nodiscard]] bool is_loaded() const
  {
    ensure_loaded();
    return loaded;
  }
  [[nodiscard]] bool is_dirty() const
  {
    ensure_loaded();
    return dirty;
  }
  [[nodiscard]]
  std::optional<QSqlQuery> get_rent_references() const;

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
  void set_charge_advance(float _charge_advance)
  {
    dirty          = true;
    charge_advance = _charge_advance;
  }
  void set_due(EDue _due_id)
  {
    dirty  = true;
    due_id = _due_id;
  }
  void set_pay_date(const QDate& _pay_date)
  {
    dirty    = true;
    pay_date = _pay_date;
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
  int id = -1;

  mutable bool loaded = false;
  mutable bool dirty  = false;

  mutable int     tenant_id = -1;
  mutable QDate   start_date;
  mutable QDate   end_date;
  mutable float   charge_advance = 0.0f;
  mutable EDue    due_id;
  mutable QDate   pay_date;
  mutable QString comment;
};

#endif // RECEIPT_H
