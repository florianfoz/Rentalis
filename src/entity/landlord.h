#ifndef LANDLORD_H
#define LANDLORD_H

#include "base.h"
#include "record/field/field.h"
#include "record/record.h"

#include <QDate>
#include <QString>

class Landlord : public Record<Landlord>
{
public:
  static constexpr ETable static_table = ETable::Landlord;

  [[nodiscard]] QString full_name() const
  {
    return ETitle_to_str(title) + " " + first_name + " " + last_name;
  }

  QString     first_name;
  QString     last_name;
  ETitle      title;
  EEntityType entity_type;
  QString     email;
  QString     phone;
  QString     address;
  QDate       birthday;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"first_name", &Landlord::first_name),   //
        field(u"last_name", &Landlord::last_name),     //
        field(u"title", &Landlord::title),             //
        field(u"entity_type", &Landlord::entity_type), //
        field(u"email", &Landlord::email),             //
        field(u"phone", &Landlord::phone),             //
        field(u"address", &Landlord::address),         //
        field(u"birthday", &Landlord::birthday),       //
    };
  }

  bool operator==(const Landlord& other) const noexcept
  {
    return first_name == other.first_name &&   //
           last_name == other.last_name &&     //
           title == other.title &&             //
           entity_type == other.entity_type && //
           email == other.email &&             //
           phone == other.phone &&             //
           address == other.address &&         //
           birthday == other.birthday;
  }
};

#endif // LANDLORD_H
