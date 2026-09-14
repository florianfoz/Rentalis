#ifndef RENT_H
#define RENT_H

#include "record/field/field.h"
#include "record/record.h"

class Rent : public Record<Rent>
{
public:
  static constexpr ETable static_table = ETable::Rent;

  [[nodiscard]] static Rent load_from_tenant(int _tenant_id, const QDate& start_date);
  [[nodiscard]] static Rent load_from_property(int _property_id, const QDate& start_date);

  QDate   date;
  float   rent            = 0.0F;
  float   housing_aid     = 0.0F;
  float   charge          = 0.0F;
  float   household_waste = 0.0F;
  int     property_id     = 0;
  int     tenant_id       = 0;
  QString comment;

  static constexpr auto sql_fields()
  {
    return std::tuple{
        field(u"date", &Rent::date),                       //
        field(u"rent", &Rent::rent),                       //
        field(u"housing_aid", &Rent::housing_aid),         //
        field(u"charge", &Rent::charge),                   //
        field(u"household_waste", &Rent::household_waste), //
        field(u"property_id", &Rent::property_id),         //
        field(u"tenant_id", &Rent::tenant_id),             //
        field(u"comment", &Rent::comment),                 //
    };
  }
};

#endif // RENT_H
