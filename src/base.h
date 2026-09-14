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
            Due, 2,        //
            WillDue, 3,    //
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
            Mrs_Mr, 2,       //
            Mr, 3,           //
            Mrs, 4,          //
            Damsel, 5,       //
            Squire, 6,       //
)

DEFINE_ENUM(EEntityType, uint8_t,  //
            Individual, 2,         //
            Family, 3,             //
            CoTenant, 4,           //
            Company, 5,            //
            Organization, 6,       //
            TemporaryOccupant, 7,  //
            PrivateInstitution, 8, //
            PublicInstitution, 9,  //
            PrivateCollective, 10, //
            PublicCollective, 11,  //
            PublicServices, 12,    //
)

DEFINE_ENUM(EPropertyType, uint8_t, //
            Unknown, 2,             //
            Studio, 3,              //
            StudioPlus, 4,          //
            _1B, 5,                 //
            _2B, 6,                 //
            _3B, 7,                 //
            _4B, 8,                 //
            _5BPlus, 9,             //
)

DEFINE_ENUM(ERoomType, uint8_t,  //
            Other, 2,            //
            LivingRoom, 3,       //
            Bedroom, 4,          //
            Kitchen, 5,          //
            Bathroom, 6,         //
            DiningRoom, 7,       //
            OfficeStudy, 8,      //
            BalconyTerrace, 9,   //
            GarageParking, 10,   //
            LaundryRoom, 11,     //
            StorageCellar, 12,   //
            HallwayEntrance, 13, //
            GuestRoom, 14,       //
            Closet, 15,          //
            Sauna, 16,           //
)

DEFINE_ENUM(ECondition, uint8_t, //
            Unknown, 2,          //
            Uninhabitable, 3,    //
            Poor, 4,             //
            Deteriorated, 5,     //
            Fair, 6,             //
            Good, 7,             //
            Excellent, 8,        //
            New, 9,              //
)

DEFINE_ENUM(EEnergy, uint8_t, //
            Unknown, 2,       //
            G, 3,             //
            F, 4,             //
            E, 5,             //
            D, 6,             //
            C, 7,             //
            B, 8,             //
            A, 9,             //
)


DEFINE_ENUM(EFeatureType, uint8_t, //
            Garden, 2,             //
            Terrace, 3,            //
            Balcony, 4,            //
            Garage, 5,             //
            Elevator, 6,           //
            Cellar, 7,             //
            Parking, 8,            //
            Basement, 9,           //
            Pool, 10,              //
            Shed, 11,              //
)


DEFINE_ENUM(EStatus, uint8_t, //
            Reported, 2,      //
            Assessment, 3,    //
            InProgress, 4,    //
            Inspection, 5,    //
            Completed, 6,     //
)


DEFINE_ENUM(ESeverity, uint8_t, //
            Unknown, 2,         //
            Minor, 3,           //
            Moderate, 4,        //
            Significant, 5,     //
            Severe, 6,          //
            Critical, 7,        //
)


DEFINE_ENUM(ELeaseType, uint8_t, //
            FixedTerm, 2,        //
            Periodic, 3,         //
            Sublease, 4,         //
            CommercialLease, 5,  //
            GroundLease, 6,      //
            RentToOwn, 7,        //
)


QString getLocaleFromFile(const QString& fileName);


QString ftod(float val);
QString itod(int val);

QString ftom(float val);
QString itom(int val);

bool EEntityType_is_human(EEntityType entity_type);


int  init_welcome();
void init_save_path();
void destroy_current_database();
bool init_database();
void init_print_path();
void init_traductions();
void init_themes();


std::unique_ptr<QPixmap> makeRoundedAvatar(const QByteArray& data, QSize size);

QString sanitize_fileName(const QString& input);

static const QHash<QString, QString> CURRENCIES = {
    {"USD", "$"  },
    {"EUR", "€"  },
    {"GBP", "£"  },
    {"JPY", "¥"  },
    {"CNY", "¥"  },
    {"CHF", "CHF"},
    {"CAD", "$"  },
    {"AUD", "$"  },
    {"NZD", "$"  },
    {"HKD", "$"  },
    {"SGD", "$"  },
    {"KRW", "₩"  },
    {"INR", "₹"  },
    {"MXN", "$"  },
    {"BRL", "R$" },
    {"SEK", "kr" },
    {"NOK", "kr" },
    {"DKK", "kr" },
    {"PLN", "zł" },
    {"ZAR", "R"  }
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
    "ru", // Russian
    "uk", // Ukrainian
    "tr", // Turkish
    "ar", // Arabic
    "he", // Hebrew
    "fa", // Persian
    "hi", // Hindi
    "bn", // Bengali
    "zh", // Chinese
    "ja", // Japanese
    "ko", // Korean
    "th", // Thai
    "vi", // Vietnamese
    "id", // Indonesian
};


inline bool WAIT_DATABASE_TO_START = true;

#endif // BASE_H
