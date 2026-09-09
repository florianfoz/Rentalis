#ifndef BASE_H
#define BASE_H

#include "database_manager.h"

#include <QSize>
#include <QStandardPaths>
#include <QString>

class QApplication;
class QPixmap;

inline QString DATABASE_PATH()
{
  static const QString path =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/data/rentalis_database.sqlite";
  return path;
}
inline QString SAVE_PATH()
{
  static const QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/save";
  // sub generation ../databases
  return path;
}
inline QString PRINT_PATH()
{
  static const QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/print_output";
  return path;
}
inline QString TRADUCTION_PATH()
{
  static const QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/translations";
  // sub generation ../software ../user
  return path;
}
inline QString THEME_PATH()
{
  static const QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/themes";
  // subgeneration ../software ../user
  return path;
}
inline QString          THEME_LIGHT_CSS;
inline QString          THEME_DARK_CSS;
inline Database_Manager DB_MANAGER;
inline Settings         SETTINGS;
inline QApplication*    APP;

namespace TXT
{
extern const QString ABOUT;
extern const QString COPYRIGHTS_LABEL;

extern const QString WARNING_OPERATION;

extern const QString INVOICE_MAIL_SUBJECT;
extern const QString INVOICE_MAIL_BODY;

extern const QString RECEIPT_MAIL_SUBJECT;
extern const QString RECEIPT_MAIL_BODY;
} // namespace TXT


enum class EMode { Edit, Creation, View };

enum class EDue { None, Due, WillDue };

enum class EMonth {
  None,
  January,
  February,
  March,
  April,
  May,
  June,
  July,
  August,
  September,
  October,
  November,
  December,
};

constexpr EMonth all_months[] = {
    EMonth::January, EMonth::February, EMonth::March,     EMonth::April,   EMonth::May,      EMonth::June,
    EMonth::July,    EMonth::August,   EMonth::September, EMonth::October, EMonth::November, EMonth::December,
};

enum class ETitle {
  None,
  Mrs_Mr,
  Mr,
  Mrs,
  Damsel,
  Squire,
};

constexpr ETitle all_titles[] = {
    ETitle::Mrs_Mr, ETitle::Mr, ETitle::Mrs, ETitle::Damsel, ETitle::Squire,
};

enum class EEntityType {
  None,
  Individual,
  Family,
  CoTenant,
  Company,
  Organization,
  TemporaryOccupant,
  PrivateInstitution,
  PublicInstitution,
  PrivateCollective,
  PublicCollective,
  PublicServices,
};

constexpr EEntityType all_entity_types[] = {
    EEntityType::Individual,         EEntityType::Family,
    EEntityType::CoTenant,           EEntityType::Company,
    EEntityType::Organization,       EEntityType::TemporaryOccupant,
    EEntityType::PrivateInstitution, EEntityType::PublicInstitution,
    EEntityType::PrivateCollective,  EEntityType::PublicCollective,
    EEntityType::PublicServices,
};

enum class EPropertyType {
  None,
  Unknown,
  Studio,
  StudioPlus,
  _1B,
  _2B,
  _3B,
  _4B,
  _5BPlus,
};

constexpr EPropertyType all_property_types[] = {
    EPropertyType::Unknown, EPropertyType::Studio, EPropertyType::StudioPlus, EPropertyType::_1B,
    EPropertyType::_2B,     EPropertyType::_3B,    EPropertyType::_4B,        EPropertyType::_5BPlus,
};

enum class ERoomType {
  None,
  Other,
  LivingRoom,
  Bedroom,
  Kitchen,
  Bathroom,
  DiningRoom,
  OfficeStudy,
  BalconyTerrace,
  GarageParking,
  LaundryRoom,
  StorageCellar,
  HallwayEntrance,
  GuestRoom,
  Closet,
  Sauna,
};

constexpr ERoomType all_room_types[] = {
    ERoomType::Other,         ERoomType::LivingRoom,  ERoomType::Bedroom,       ERoomType::Kitchen,
    ERoomType::Bathroom,      ERoomType::DiningRoom,  ERoomType::OfficeStudy,   ERoomType::BalconyTerrace,
    ERoomType::GarageParking, ERoomType::LaundryRoom, ERoomType::StorageCellar, ERoomType::HallwayEntrance,
    ERoomType::GuestRoom,     ERoomType::Closet,      ERoomType::Sauna,
};

enum class ECondition {
  None,
  Unknown,
  Uninhabitable,
  Poor,
  Deteriorated,
  Fair,
  Good,
  Excellent,
  New,
};

constexpr ECondition all_conditions[] = {
    ECondition::Unknown, ECondition::Uninhabitable, ECondition::Poor,      ECondition::Deteriorated,
    ECondition::Fair,    ECondition::Good,          ECondition::Excellent, ECondition::New,
};

enum class EEnergy {
  None,
  Unknown,
  G,
  F,
  E,
  D,
  C,
  B,
  A,
};

constexpr EEnergy all_energy[] = {
    EEnergy::Unknown, EEnergy::G, EEnergy::F, EEnergy::E, EEnergy::D, EEnergy::C, EEnergy::B, EEnergy::A,
};

enum class EFeatureType {
  None,
  Garden,
  Terrace,
  Balcony,
  Garage,
  Elevator,
  Cellar,
  Parking,
  Basement,
  Pool,
  Shed,
};

constexpr EFeatureType all_feature_types[] = {
    EFeatureType::Garden, EFeatureType::Terrace, EFeatureType::Balcony,  EFeatureType::Garage, EFeatureType::Elevator,
    EFeatureType::Cellar, EFeatureType::Parking, EFeatureType::Basement, EFeatureType::Pool,   EFeatureType::Shed,
};


enum class EStatus {
  None,
  Reported,
  Assessment,
  InProgress,
  Inspection,
  Completed,
};

constexpr EStatus all_status[] = {
    EStatus::Reported, EStatus::Assessment, EStatus::InProgress, EStatus::Inspection, EStatus::Completed,
};


enum class ESeverity { None, Unknown, Minor, Moderate, Significant, Severe, Critical };

constexpr ESeverity all_severity[] = {ESeverity::Unknown,     ESeverity::Minor,  ESeverity::Moderate,
                                      ESeverity::Significant, ESeverity::Severe, ESeverity::Critical};


enum class ELeaseType {
  None,
  FixedTerm,
  Periodic,
  Sublease,
  CommercialLease,
  GroundLease,
  RentToOwn,
};

constexpr ELeaseType all_lease_types[] = {
    ELeaseType::FixedTerm,       ELeaseType::Periodic,    ELeaseType::Sublease,
    ELeaseType::CommercialLease, ELeaseType::GroundLease, ELeaseType::RentToOwn,
};

QString getLocaleFromFile(const QString& fileName);


QString ftod(float val);
QString itod(int val);

QString ftom(float val);
QString itom(int val);

QString EDue_to_str(EDue due);
EDue    int_to_EDue(int due);

QString EMonth_to_str(EMonth month);
EMonth  int_to_EMonth(int month);

QString ETitle_to_str(ETitle title);
ETitle  int_to_ETitle(int title_id);

QString     EEntityType_to_str(EEntityType entity_type);
EEntityType int_to_EEntityType(int entity_type);
bool        is_EEntityType_is_human(EEntityType entity_type);

QString       EPropertyType_to_str(EPropertyType property_type);
EPropertyType int_to_EPropertyType(int property_type);

QString   ERoomType_to_str(ERoomType room_type);
ERoomType int_to_ERoomType(int room_type);

QString    ECondition_to_str(ECondition condition);
ECondition int_to_ECondition(int condition);

QString EEnergy_to_str(EEnergy energy);
EEnergy int_to_EEnergy(int energy);

QString      EFeatureType_to_str(EFeatureType property_feature);
EFeatureType int_to_EFeatureType(int property_feature);

QString EStatus_to_str(EStatus status);
EStatus int_to_EStatus(int status);

QString   ESeverity_to_str(ESeverity severity);
ESeverity int_to_EServerity(int serverity);

QString    ELeaseType_to_str(ELeaseType lease_type);
ELeaseType int_to_ELeaseType(int lease_type);


void init_save_path();
void destroy_current_database();
bool init_database();
void init_print_path();
void init_traductions();
void init_themes();
void init_settings();


std::unique_ptr<QPixmap> makeRoundedAvatar(const QByteArray& data, QSize size);

QString sanitize_fileName(const QString& input);

#endif // BASE_H
