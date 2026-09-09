#include "base.h"

#include "database.h"
#include "property.h"
#include "settings.h"
#include "widget/w_database_creator.h"

#include <QComboBox>
#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QLocale>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QRegularExpression>
#include <QSql>
#include <QSqlDatabase>
#include <QStandardPaths>

namespace TXT
{
const QString ABOUT = QObject::tr(
    "<b>Rentalis – Complete Property Management</b><br><br>"
    "Manage your rents, properties, tenants, landlords, invoices and receipts.<br>"
    "- Auto invoices and receipts printer<br>"
    "- Data manager (SQL Base)"
    "- Save data manager<br>"
    "- Auto EMail writer<br>"
    "- Auto prompt generation for desired AI analytic<br>"
    "- Tax calculation and configurable charts<br><br>"
    "© 2026 Foz Florian All rights reserved.<br>"
    "Rentalis is licensed under the GNU Affero General Public License<br>"
    "version 3 or any later version.<br><br>"
    "Some icons used in this software come from "
    "<a href='https://github.com/PapirusDevelopmentTeam/papirus-icon-theme'>Papirus Icons</a>.<br>"
    "This application uses the Qt 6 framework, which is licensed under the LGPL v3.<br>"
    "See <a href='https://www.qt.io/licensing'>Qt6 Software</a> for more information.<br><br>");

const QString COPYRIGHTS_LABEL = QObject::tr("Rentalis - © 2025 Foz Florian. All Rights Reserved.");

const QString WARNING_OPERATION =
    QObject::tr("\nThis action is irreversible; please make backups before performing any dangerous operation.");

const QString NOT_FOUND = QObject::tr("Not Found !");

const QString INVOICE_MAIL_SUBJECT = QObject::tr("LOCATION - Invoice of [%1] to [%2] at [%3]");

const QString INVOICE_MAIL_BODY = QObject::tr(
    "Hello %1,\n\n"
    "Please find attached the rental invoice dated from %2 to %3.\n"
    "Property located at %4.\n\n"
    "Please proceed with the payment according to the usual terms if it has not been done yet.\n\n"
    "Best regards,\n"
    "The landlords %5");

const QString RECEIPT_MAIL_SUBJECT = QObject::tr("LOCATION - Receipt of [%1] to [%2] at [%3]");

const QString RECEIPT_MAIL_BODY = QObject::tr(
    "Hello %1,\n\n"
    "Please find attached the rental receipt dated from %2 to %3.\n"
    "Property located at %4.\n\n"
    "Best regards,\n"
    "The landlords %5");
} // namespace TXT

QString getLocaleFromFile(const QString& fileName)
{
  QString baseName = QFileInfo(fileName).baseName();

  int underscoreIndex = baseName.lastIndexOf('_');
  if (underscoreIndex != -1) {
    QString locale = baseName.mid(underscoreIndex + 1);
    return locale;
  }

  return QString();
}

QString ftod(float val)
{

  return SETTINGS.get_locale().toString(val);
}

QString itod(int val)
{

  return SETTINGS.get_locale().toString(val);
}

QString ftom(float val)
{

  if (!DB_MANAGER.is_valid()) return SETTINGS.get_locale().toCurrencyString(val, "¤", 2);
  return SETTINGS.get_locale().toCurrencyString(val, DB_MANAGER.get_db()->get_settings().get_currency().symbol, 2);
}

QString itom(int val)
{

  if (!DB_MANAGER.is_valid()) return SETTINGS.get_locale().toCurrencyString(val, "¤");
  return SETTINGS.get_locale().toCurrencyString(val, DB_MANAGER.get_db()->get_settings().get_currency().symbol);
}


// start to 1
QString EMonth_to_str(EMonth month)
{
  switch (month) {
  case EMonth::January:   return QObject::tr("January");
  case EMonth::February:  return QObject::tr("February");
  case EMonth::March:     return QObject::tr("March");
  case EMonth::April:     return QObject::tr("April");
  case EMonth::May:       return QObject::tr("May");
  case EMonth::June:      return QObject::tr("June");
  case EMonth::July:      return QObject::tr("July");
  case EMonth::August:    return QObject::tr("August");
  case EMonth::September: return QObject::tr("September");
  case EMonth::October:   return QObject::tr("October");
  case EMonth::November:  return QObject::tr("November");
  case EMonth::December:  return QObject::tr("December");
  default:                return QObject::tr("Invalid month");
  }
}

EMonth int_to_EMonth(int month)
{
  switch (month) {
  case 1:  return EMonth::January;
  case 2:  return EMonth::February;
  case 3:  return EMonth::March;
  case 4:  return EMonth::April;
  case 5:  return EMonth::May;
  case 6:  return EMonth::June;
  case 7:  return EMonth::July;
  case 8:  return EMonth::August;
  case 9:  return EMonth::September;
  case 10: return EMonth::October;
  case 11: return EMonth::November;
  case 12: return EMonth::December;
  default: return EMonth::None;
  }
}

QString EDue_to_str(EDue due)
{
  switch (due) {
  case EDue::Due:     return QObject::tr("Due");
  case EDue::WillDue: return QObject::tr("Will due");
  default:            return QObject::tr("Invalid due_id");
  }
}

EDue int_to_EDue(int due)
{
  switch (due) {
  case 1:  return EDue::Due;
  case 2:  return EDue::WillDue;
  default: return EDue::None;
  }
}


QString ETitle_to_str(ETitle title)
{
  switch (title) {
  case ETitle::Mrs_Mr: return QObject::tr("Mrs. Mr.");
  case ETitle::Mr:     return QObject::tr("Mr.");
  case ETitle::Mrs:    return QObject::tr("Mrs.");
  case ETitle::Damsel: return QObject::tr("Damsel");
  case ETitle::Squire: return QObject::tr("Squire");
  default:             return QObject::tr("Invalid title");
  }
}

ETitle int_to_ETitle(int title)
{
  switch (title) {
  case 1:  return ETitle::Mrs_Mr;
  case 2:  return ETitle::Mr;
  case 3:  return ETitle::Mrs;
  case 4:  return ETitle::Damsel;
  case 5:  return ETitle::Squire;
  default: return ETitle::None;
  }
}

QString EEntityType_to_str(EEntityType entity_type)
{
  switch (entity_type) {
  case EEntityType::Individual:         return QObject::tr("Individual");
  case EEntityType::Family:             return QObject::tr("Family");
  case EEntityType::CoTenant:           return QObject::tr("Co-tenant");
  case EEntityType::Company:            return QObject::tr("Company");
  case EEntityType::Organization:       return QObject::tr("Organization");
  case EEntityType::TemporaryOccupant:  return QObject::tr("Temporary occupant");
  case EEntityType::PrivateInstitution: return QObject::tr("Private institution");
  case EEntityType::PublicInstitution:  return QObject::tr("Public institution");
  case EEntityType::PrivateCollective:  return QObject::tr("Private collective");
  case EEntityType::PublicCollective:   return QObject::tr("Public collective");
  case EEntityType::PublicServices:     return QObject::tr("Public services");
  default:                              return QObject::tr("Invalid entity_type");
  }
}

EEntityType int_to_EEntityType(int entity_type)
{
  switch (entity_type) {
  case 1:  return EEntityType::Individual;
  case 2:  return EEntityType::Family;
  case 3:  return EEntityType::CoTenant;
  case 4:  return EEntityType::Company;
  case 5:  return EEntityType::Organization;
  case 6:  return EEntityType::TemporaryOccupant;
  case 7:  return EEntityType::PrivateInstitution;
  case 8:  return EEntityType::PublicInstitution;
  case 9:  return EEntityType::PrivateCollective;
  case 10: return EEntityType::PublicCollective;
  case 11: return EEntityType::PublicServices;
  default: return EEntityType::None;
  }
}


bool is_EEntityType_is_human(EEntityType entity_type)
{
  switch (entity_type) {
  case EEntityType::Individual:         return true;
  case EEntityType::Family:             return true;
  case EEntityType::CoTenant:           return true;
  case EEntityType::Company:            return false;
  case EEntityType::Organization:       return false;
  case EEntityType::TemporaryOccupant:  return false;
  case EEntityType::PrivateInstitution: return false;
  case EEntityType::PublicInstitution:  return false;
  case EEntityType::PrivateCollective:  return false;
  case EEntityType::PublicCollective:   return false;
  case EEntityType::PublicServices:     return false;
  default:                              return false;
  }
}


QString EPropertyType_to_str(EPropertyType property_type)
{
  switch (property_type) {
  case EPropertyType::Studio:     return QObject::tr("Studio");
  case EPropertyType::StudioPlus: return QObject::tr("Studio +");
  case EPropertyType::_1B:        return QObject::tr("1B");
  case EPropertyType::_2B:        return QObject::tr("2B");
  case EPropertyType::_3B:        return QObject::tr("3B");
  case EPropertyType::_4B:        return QObject::tr("4B");
  case EPropertyType::_5BPlus:    return QObject::tr("5B +");
  default:                        return QObject::tr("Invalid property_type");
  }
}


EPropertyType int_to_EPropertyType(int property_type)
{
  switch (property_type) {
  case 1:  return EPropertyType::Studio;
  case 2:  return EPropertyType::StudioPlus;
  case 3:  return EPropertyType::_1B;
  case 4:  return EPropertyType::_2B;
  case 5:  return EPropertyType::_3B;
  case 6:  return EPropertyType::_4B;
  case 7:  return EPropertyType::_5BPlus;
  default: return EPropertyType::None;
  }
}

QString ECondition_to_str(ECondition condition)
{
  switch (condition) {
  case ECondition::New:           return QObject::tr("New");
  case ECondition::Excellent:     return QObject::tr("Excellent");
  case ECondition::Good:          return QObject::tr("Good");
  case ECondition::Fair:          return QObject::tr("Fair");
  case ECondition::Deteriorated:  return QObject::tr("Deteriorated");
  case ECondition::Poor:          return QObject::tr("Poor");
  case ECondition::Uninhabitable: return QObject::tr("Uninhabitable");
  default:                        return QObject::tr("Invalid condition");
  }
}

ECondition int_to_ECondition(int condition)
{
  switch (condition) {
  case 1:  return ECondition::New;
  case 2:  return ECondition::Excellent;
  case 3:  return ECondition::Good;
  case 4:  return ECondition::Fair;
  case 5:  return ECondition::Deteriorated;
  case 6:  return ECondition::Poor;
  case 7:  return ECondition::Uninhabitable;
  default: return ECondition::None;
  }
}

QString EEnergy_to_str(EEnergy energy)
{
  switch (energy) {
  case EEnergy::A: return "A";
  case EEnergy::B: return "B";
  case EEnergy::C: return "C";
  case EEnergy::D: return "D";
  case EEnergy::E: return "E";
  case EEnergy::F: return "F";
  case EEnergy::G: return "G";
  default:         return "Invalid energy";
  }
}

EEnergy int_to_EEnergy(int energy)
{
  switch (energy) {
  case 1:  return EEnergy::A;
  case 2:  return EEnergy::B;
  case 3:  return EEnergy::C;
  case 4:  return EEnergy::D;
  case 5:  return EEnergy::E;
  case 6:  return EEnergy::F;
  case 7:  return EEnergy::G;
  default: return EEnergy::None;
  }
}


QString ERoomType_to_str(ERoomType room_type)
{
  switch (room_type) {
  case ERoomType::LivingRoom:      return QObject::tr("Living room");
  case ERoomType::Bedroom:         return QObject::tr("Bedroom");
  case ERoomType::Kitchen:         return QObject::tr("Kitchen");
  case ERoomType::Bathroom:        return QObject::tr("Bathroom");
  case ERoomType::DiningRoom:      return QObject::tr("Dining room");
  case ERoomType::OfficeStudy:     return QObject::tr("Office / Study");
  case ERoomType::BalconyTerrace:  return QObject::tr("Balcony / Terrace");
  case ERoomType::GarageParking:   return QObject::tr("Garage / Parking");
  case ERoomType::LaundryRoom:     return QObject::tr("Laundry room");
  case ERoomType::StorageCellar:   return QObject::tr("Storage / Cellar");
  case ERoomType::HallwayEntrance: return QObject::tr("Hallway / Entrance");
  case ERoomType::GuestRoom:       return QObject::tr("Guest room");
  case ERoomType::Closet:          return QObject::tr("Closet");
  default:                         return QObject::tr("Invalid room_type");
  }
}

ERoomType int_to_ERoomType(int room_type)
{
  switch (room_type) {
  case 1:  return ERoomType::LivingRoom;
  case 2:  return ERoomType::Bedroom;
  case 3:  return ERoomType::Kitchen;
  case 4:  return ERoomType::Bathroom;
  case 5:  return ERoomType::DiningRoom;
  case 6:  return ERoomType::OfficeStudy;
  case 7:  return ERoomType::BalconyTerrace;
  case 8:  return ERoomType::GarageParking;
  case 9:  return ERoomType::LaundryRoom;
  case 10: return ERoomType::StorageCellar;
  case 11: return ERoomType::HallwayEntrance;
  case 12: return ERoomType::GuestRoom;
  case 13: return ERoomType::Closet;
  default: return ERoomType::None;
  }
}


QString EFeatureType_to_str(EFeatureType property_feature)
{
  switch (property_feature) {
  case EFeatureType::Garden:   return QObject::tr("Garden");
  case EFeatureType::Terrace:  return QObject::tr("Terrace");
  case EFeatureType::Balcony:  return QObject::tr("Balcony");
  case EFeatureType::Garage:   return QObject::tr("Garage");
  case EFeatureType::Elevator: return QObject::tr("Elevator");
  case EFeatureType::Cellar:   return QObject::tr("Cellar");
  case EFeatureType::Parking:  return QObject::tr("Parking");
  case EFeatureType::Basement: return QObject::tr("Basement");
  case EFeatureType::Pool:     return QObject::tr("Pool");
  case EFeatureType::Shed:     return QObject::tr("Shed");
  default:                     return QObject::tr("Invalid room_type");
  }
}

EFeatureType int_to_EFeatureType(int property_feature)
{
  switch (property_feature) {
  case 1:  return EFeatureType::Garden;
  case 2:  return EFeatureType::Terrace;
  case 3:  return EFeatureType::Balcony;
  case 4:  return EFeatureType::Garage;
  case 5:  return EFeatureType::Elevator;
  case 6:  return EFeatureType::Cellar;
  case 7:  return EFeatureType::Parking;
  case 8:  return EFeatureType::Basement;
  case 9:  return EFeatureType::Pool;
  case 10: return EFeatureType::Shed;
  default: return EFeatureType::None;
  }
}


QString EStatus_to_str(EStatus status)
{
  switch (status) {
  case EStatus::Reported:   return QObject::tr("Reported");
  case EStatus::Assessment: return QObject::tr("Assessment");
  case EStatus::InProgress: return QObject::tr("In progress");
  case EStatus::Inspection: return QObject::tr("Inspection");
  case EStatus::Completed:  return QObject::tr("Completed");
  default:                  return QObject::tr("Invalid status");
  }
}

EStatus int_to_EStatus(int status)
{
  switch (status) {
  case 1:  return EStatus::Reported;
  case 2:  return EStatus::Assessment;
  case 3:  return EStatus::InProgress;
  case 4:  return EStatus::Inspection;
  case 5:  return EStatus::Completed;
  default: return EStatus::None;
  }
}

QString ESeverity_to_str(ESeverity severity)
{
  switch (severity) {
  case ESeverity::Minor:       return QObject::tr("Minor");
  case ESeverity::Moderate:    return QObject::tr("Assessment");
  case ESeverity::Significant: return QObject::tr("Significant");
  case ESeverity::Severe:      return QObject::tr("Servere");
  case ESeverity::Critical:    return QObject::tr("Critical");
  default:                     return QObject::tr("Invalid serverity");
  }
}

ESeverity int_to_EServerity(int serverity)
{
  switch (serverity) {
  case 1:  return ESeverity::Minor;
  case 2:  return ESeverity::Moderate;
  case 3:  return ESeverity::Significant;
  case 4:  return ESeverity::Severe;
  case 5:  return ESeverity::Critical;
  default: return ESeverity::None;
  }
}

QString ELeaseType_to_str(ELeaseType lease_type)
{
  switch (lease_type) {
  case ELeaseType::FixedTerm:       return QObject::tr("Fixed term");
  case ELeaseType::Periodic:        return QObject::tr("Periodic");
  case ELeaseType::Sublease:        return QObject::tr("Sublease");
  case ELeaseType::CommercialLease: return QObject::tr("Commercial lease");
  case ELeaseType::GroundLease:     return QObject::tr("Ground lease");
  default:                          return QObject::tr("Invalid lease_type");
  }
}

ELeaseType int_to_ELeaseType(int lease_type)
{
  switch (lease_type) {
  case 1:  return ELeaseType::FixedTerm;
  case 2:  return ELeaseType::Periodic;
  case 3:  return ELeaseType::Sublease;
  case 4:  return ELeaseType::CommercialLease;
  case 5:  return ELeaseType::GroundLease;
  default: return ELeaseType::None;
  }
}

void init_save_path()
{
  QDir().mkpath(SAVE_PATH());
}

bool init_database()
{
  // if no database existing : open the database creator
  if (SETTINGS.get_last_db().isEmpty() || !QFile::exists(SETTINGS.get_last_db())) {
    auto db_creator = W_Database_Creator(nullptr, "");
    db_creator.setModal(true);
    db_creator.exec();
    return db_creator.is_creation_successful;
  }

  auto db = std::make_unique<Database>();
  if (!db->open_database(SETTINGS.get_last_db())) return false;
  return DB_MANAGER.change_database(std::move(db));
}

void init_print_path()
{
  QDir().mkpath(PRINT_PATH());
}

void init_traductions()
{
  QDir().mkpath(TRADUCTION_PATH());
  QDir().mkpath(TRADUCTION_PATH() + "/software");
  QDir().mkpath(TRADUCTION_PATH() + "/user");

  // generate traduction template
  {
    QString target_template = TRADUCTION_PATH() + "/software/Rentalis_template.ts";
    QFile   file(":/translations/Rentalis_en.ts");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QFile out(target_template);
      if (out.open(QIODevice::WriteOnly | QIODevice::Text)) {
        out.write(file.readAll());
        out.close();
      }
      file.close();
    }
  }
}

void init_themes()
{
  QDir().mkpath(THEME_PATH());
  QDir().mkpath(THEME_PATH() + "/software");
  QDir().mkpath(THEME_PATH() + "/user");

  // generate software theme template
  {
    QFile l_file(":/themes/rentalis-light.css");
    if (l_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QTextStream stream(&l_file);
      THEME_LIGHT_CSS = stream.readAll();
      l_file.close();
    }
    QFile d_file(":/themes/rentalis-dark.css");
    if (d_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QTextStream stream(&d_file);
      THEME_DARK_CSS = stream.readAll();
      d_file.close();
    }
  }

  // generate theme template
  {
    QString target_template = THEME_PATH() + "/software/rentalis-template.css";
    QFile   file(":/themes/rentalis-light.css");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QFile out(target_template);
      if (out.open(QIODevice::WriteOnly | QIODevice::Text)) {
        out.write(file.readAll());
        out.close();
      }
      file.close();
    }
  }
}

void init_settings()
{
  SETTINGS = Settings();
}

std::unique_ptr<QPixmap> makeRoundedAvatar(const QByteArray& data, QSize size)
{
  QPixmap source;
  source.loadFromData(data);

  if (source.isNull()) source.load(":/images/default_avatar.png");

  QPixmap scaled = source.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  QPixmap rounded(size);
  rounded.fill(Qt::transparent);

  QPainter painter(&rounded);
  painter.setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  path.addEllipse(rounded.rect());
  painter.setClipPath(path);
  painter.drawPixmap(0, 0, scaled);

  return std::make_unique<QPixmap>(rounded);
}


QString sanitize_fileName(const QString& input)
{
  QString name = input.trimmed();

  // unicode -> ASCII
  name = name.normalized(QString::NormalizationForm_D);
  name.remove(QRegularExpression("[\\u0300-\\u036f]"));

  name = name.toLower();
  // replace special char to "_"
  name.replace(QRegularExpression(R"([\\\/:*?"<>|])"), "_");
  // delete control char
  name.remove(QRegularExpression(R"([\x00-\x1F])"));
  // replace other char to "_"
  name.replace(QRegularExpression(R"([^a-z0-9._-]+)"), "_");
  // replace multiple char to one
  name.replace(QRegularExpression(R"(_{2,})"), "_");

  // delete "_" "." in start / end
  name.remove(QRegularExpression(R"(^[._]+|[._]+$)"));

  // max length
  const int maxLength = 255;
  if (name.length() > maxLength) name = name.left(maxLength);

  // fallback if empty
  if (name.isEmpty()) name = "file";

  return name;
}
