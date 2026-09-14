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

  QString full_name() const
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
  QString     singing;
  QString     icon;

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
        field(u"singing", &Landlord::singing),         //
        field(u"icon", &Landlord::icon),               //
    };
  }
};

#endif // LANDLORD_H
