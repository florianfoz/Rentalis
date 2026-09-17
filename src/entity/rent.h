#ifndef RENT_H
#define RENT_H

#include "record/field/field.h"
#include "record/record.h"

class Rent : public Record<Rent>
{
public:
  static constexpr ETable static_table = ETable::Rent;

  [[nodiscard]] static Rent load_from_tenant(qsizetype _tenant_id, const QDate& start_date);
  [[nodiscard]] static Rent load_from_property(qsizetype _property_id, const QDate& start_date);

  [[nodiscard]] static QList<std::tuple<int, qsizetype>> find_by_year_property(int year, qsizetype property_id);
  [[nodiscard]] static qsizetype is_exists(EMonth month, int year, qsizetype property_id, bool msg);
  [[nodiscard]] static std::tuple<float, float, float, float> year_sum(int year, qsizetype property_id);
  [[nodiscard]] static std::optional<QSqlQuery> find_by_tenant_date(qsizetype tenant_id, const QDate& start_date,
                                                                    const QDate& end_date);

  QDate     date;
  float     rent            = 0.0F;
  float     housing_aid     = 0.0F;
  float     charge          = 0.0F;
  float     household_waste = 0.0F;
  qsizetype property_id     = INVALID_ID;
  qsizetype tenant_id       = INVALID_ID;
  QString   comment;

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

  bool operator==(const Rent& other) const noexcept
  {
    return date == other.date &&                       //
           rent == other.rent &&                       //
           housing_aid == other.housing_aid &&         //
           charge == other.charge &&                   //
           household_waste == other.household_waste && //
           property_id == other.property_id &&         //
           tenant_id == other.tenant_id &&             //
           comment == other.comment;
  }
};

#endif // RENT_H
