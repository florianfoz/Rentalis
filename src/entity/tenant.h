#ifndef TENANT_H
#define TENANT_H

#include "base.h"
#include "record/field/field.h"
#include "record/record.h"

#include <QDate>
#include <QString>

enum class EEntityType : uint8_t;
enum class ETitle : uint8_t;

class Tenant : public Record<Tenant>
{
public:
  static constexpr ETable static_table = ETable::Tenant;

  [[nodiscard]] QString get_full_name() const
  {
    return ETitle_to_str(title) + " " + first_name + " " + last_name;
  }

  [[nodiscard]] static bool is_used(qsizetype tenant_id);


  QString     first_name;
  QString     last_name;
  ETitle      title;
  QString     email;
  QString     phone;
  QDate       birthday;
  EEntityType entity_type;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"first_name", &Tenant::first_name),   //
        field(u"last_name", &Tenant::last_name),     //
        field(u"title", &Tenant::title),             //
        field(u"email", &Tenant::email),             //
        field(u"phone", &Tenant::phone),             //
        field(u"birthday", &Tenant::birthday),       //
        field(u"entity_type", &Tenant::entity_type), //
    };
  }

  bool operator==(const Tenant& other) const noexcept
  {
    return first_name == other.first_name && //
           last_name == other.last_name &&   //
           title == other.title &&           //
           email == other.email &&           //
           phone == other.phone &&           //
           birthday == other.birthday &&     //
           entity_type == other.entity_type;
  }
};

#endif // TENANT_H
