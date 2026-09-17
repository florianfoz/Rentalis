/*
 * Copyright (C) 2025 Foz Florian
 *
 * Licensed under the GNU Affero General Public License v3.0
 * or later (AGPL-3.0-or-later).
 *
 * This project uses third-party software, including Qt.
 * Third-party components remain subject to their respective licenses.
 */

#ifndef BASE_H
#define BASE_H

#include "enum_lite.h"
#include "forward.h"

#include <QSize>
#include <QStandardPaths>
#include <QString>

class QPixmap;

inline QString DATABASE_PATH()
{
  static const QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/data";
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
inline QString THEME_LIGHT_CSS;
inline QString THEME_DARK_CSS;

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


DEFINE_ENUM(EMode, uint8_t, //
            Edit, 1,        //
            Creation, 2,    //
            View, 3,        //
)

DEFINE_ENUM(EDue, uint8_t, //
            Due, 1,        //
            WillDue, 2,    //
)

DEFINE_ENUM(EMonth, uint8_t, //
            January, 1,      //
            February, 2,     //
            March, 3,        //
            April, 4,        //
            May, 5,          //
            June, 6,         //
            July, 7,         //
            August, 8,       //
            September, 9,    //
            October, 10,     //
            November, 11,    //
            December, 12,    //
)

DEFINE_ENUM(ETitle, uint8_t, //
            Mrs_Mr, 1,       //
            Mr, 2,           //
            Mrs, 3,          //
            Damsel, 4,       //
            Squire, 5,       //
)

DEFINE_ENUM(EEntityType, uint8_t,  //
            Individual, 1,         //
            Family, 2,             //
            CoTenant, 3,           //
            Company, 4,            //
            Organization, 5,       //
            TemporaryOccupant, 6,  //
            PrivateInstitution, 7, //
            PublicInstitution, 8,  //
            PrivateCollective, 9,  //
            PublicCollective, 10,  //
            PublicServices, 11,    //
)

DEFINE_ENUM(EPropertyType, uint8_t, //
            Unknown, 1,             //
            Studio, 2,              //
            StudioPlus, 3,          //
            _1B, 4,                 //
            _2B, 5,                 //
            _3B, 6,                 //
            _4B, 7,                 //
            _5BPlus, 8,             //
)

DEFINE_ENUM(ERoomType, uint8_t,  //
            Other, 1,            //
            LivingRoom, 2,       //
            Bedroom, 3,          //
            Kitchen, 4,          //
            Bathroom, 5,         //
            DiningRoom, 6,       //
            OfficeStudy, 7,      //
            BalconyTerrace, 8,   //
            GarageParking, 9,    //
            LaundryRoom, 10,     //
            StorageCellar, 11,   //
            HallwayEntrance, 12, //
            GuestRoom, 13,       //
            Closet, 14,          //
            Sauna, 15,           //
)

DEFINE_ENUM(ECondition, uint8_t, //
            Unknown, 1,          //
            Uninhabitable, 2,    //
            Poor, 3,             //
            Deteriorated, 4,     //
            Fair, 5,             //
            Good, 6,             //
            Excellent, 7,        //
            New, 8,              //
)

DEFINE_ENUM(EEnergy, uint8_t, //
            Unknown, 1,       //
            G, 2,             //
            F, 3,             //
            E, 4,             //
            D, 5,             //
            C, 6,             //
            B, 7,             //
            A, 8,             //
)


DEFINE_ENUM(EFeatureType, uint8_t, //
            Garden, 1,             //
            Terrace, 2,            //
            Balcony, 3,            //
            Garage, 4,             //
            Elevator, 5,           //
            Cellar, 6,             //
            Parking, 7,            //
            Basement, 8,           //
            Pool, 9,               //
            Shed, 10,              //
)


DEFINE_ENUM(EStatus, uint8_t, //
            Reported, 1,      //
            Assessment, 2,    //
            InProgress, 3,    //
            Inspection, 4,    //
            Completed, 5,     //
)


DEFINE_ENUM(ESeverity, uint8_t, //
            Unknown, 1,         //
            Minor, 2,           //
            Moderate, 3,        //
            Significant, 4,     //
            Severe, 5,          //
            Critical, 6,        //
)


DEFINE_ENUM(ELeaseType, uint8_t, //
            FixedTerm, 1,        //
            Periodic, 2,         //
            Sublease, 3,         //
            CommercialLease, 4,  //
            GroundLease, 5,      //
            RentToOwn, 6,        //
)


QString getLocaleFromFile(const QString& fileName);


QString ftod(float val);
QString itod(int val);

QString ftom(float val);
QString itom(int val);

bool EEntityType_is_human(EEntityType entity_type);


int  init_welcome();
void init_traductions();
void init_themes();


std::unique_ptr<QPixmap> makeRoundedAvatar(const QByteArray& data, QSize size);

QString sanitize_fileName(const QString& input);
bool    copy_dir(const QString& src, const QString& dest);


static const QHash<QString, QString> CURRENCIES = {
    {"USD", "$"   }, // US Dollar
    {"EUR", "€"   }, // Euro
    {"GBP", "£"   }, // British Pound
    {"JPY", "¥"   }, // Japanese Yen
    {"CNY", "¥"   }, // Chinese Yuan
    {"CHF", "CHF" }, // Swiss Franc
    {"FRF", "₣"   }, // French Franc (historical)

    {"CAD", "$"   }, // Canadian Dollar
    {"AUD", "$"   }, // Australian Dollar
    {"NZD", "$"   }, // New Zealand Dollar
    {"HKD", "$"   }, // Hong Kong Dollar
    {"SGD", "$"   }, // Singapore Dollar
    {"TWD", "NT$" }, // New Taiwan Dollar

    {"KRW", "₩"   }, // South Korean Won
    {"INR", "₹"   }, // Indian Rupee
    {"PKR", "₨"   }, // Pakistani Rupee
    {"BDT", "৳"   }, // Bangladeshi Taka
    {"LKR", "Rs"  }, // Sri Lankan Rupee
    {"NPR", "₨"   }, // Nepalese Rupee

    {"RUB", "₽"   }, // Russian Ruble
    {"UAH", "₴"   }, // Ukrainian Hryvnia
    {"TRY", "₺"   }, // Turkish Lira

    {"PLN", "zł"  }, // Polish Zloty
    {"CZK", "Kč"  }, // Czech Koruna
    {"HUF", "Ft"  }, // Hungarian Forint
    {"RON", "lei" }, // Romanian Leu
    {"BGN", "лв"  }, // Bulgarian Lev
    {"RSD", "дин" }, // Serbian Dinar
    {"SEK", "kr"  }, // Swedish Krona
    {"NOK", "kr"  }, // Norwegian Krone
    {"DKK", "kr"  }, // Danish Krone
    {"ISK", "kr"  }, // Icelandic Krona

    {"ILS", "₪"   }, // Israeli New Shekel
    {"AED", "د.إ" }, // United Arab Emirates Dirham
    {"SAR", "﷼"   }, // Saudi Riyal
    {"QAR", "﷼"   }, // Qatari Riyal
    {"KWD", "د.ك" }, // Kuwaiti Dinar
    {"BHD", ".د.ب"}, // Bahraini Dinar
    {"OMR", "﷼"   }, // Omani Rial
    {"JOD", "د.ا" }, // Jordanian Dinar

    {"EGP", "£"   }, // Egyptian Pound
    {"MAD", "د.م."}, // Moroccan Dirham
    {"DZD", "دج"  }, // Algerian Dinar
    {"TND", "د.ت" }, // Tunisian Dinar

    {"ZAR", "R"   }, // South African Rand
    {"NGN", "₦"   }, // Nigerian Naira
    {"KES", "KSh" }, // Kenyan Shilling
    {"GHS", "₵"   }, // Ghanaian Cedi
    {"ETB", "Br"  }, // Ethiopian Birr
    {"TZS", "TSh" }, // Tanzanian Shilling
    {"UGX", "USh" }, // Ugandan Shilling
    {"XOF", "CFA" }, // West African CFA Franc
    {"XAF", "FCFA"}, // Central African CFA Franc

    {"MXN", "$"   }, // Mexican Peso
    {"BRL", "R$"  }, // Brazilian Real
    {"ARS", "$"   }, // Argentine Peso
    {"CLP", "$"   }, // Chilean Peso
    {"COP", "$"   }, // Colombian Peso
    {"PEN", "S/"  }, // Peruvian Sol
    {"UYU", "$U"  }, // Uruguayan Peso

    {"VND", "₫"   }, // Vietnamese Dong
    {"PHP", "₱"   }, // Philippine Peso
    {"MYR", "RM"  }, // Malaysian Ringgit
    {"IDR", "Rp"  }, // Indonesian Rupiah
    {"THB", "฿"   }, // Thai Baht
};

const QStringList LANGUAGES = {
    "en", // English
    "fr", // French
    "de", // German
    "es", // Spanish
    "it", // Italian
    "pt", // Portuguese
    "nl", // Dutch
    "pl", // Polish
    "cs", // Czech
    "sk", // Slovak
    "hu", // Hungarian
    "ro", // Romanian
    "bg", // Bulgarian
    "sr", // Serbian
    "ru", // Russian
    "uk", // Ukrainian
    "sv", // Swedish
    "no", // Norwegian
    "da", // Danish
    "fi", // Finnish
    "is", // Icelandic
    "tr", // Turkish
    "el", // Greek
    "he", // Hebrew
    "ar", // Arabic
    "fa", // Persian
    "hi", // Hindi
    "bn", // Bengali
    "ur", // Urdu
    "zh", // Chinese
    "ja", // Japanese
    "ko", // Korean
    "th", // Thai
    "vi", // Vietnamese
    "id", // Indonesian
    "ms", // Malay
};


#endif // BASE_H
