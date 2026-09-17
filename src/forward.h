#ifndef FORWARD_H
#define FORWARD_H

#include "enum_lite.h"

#include <cstdint>

constexpr qsizetype INVALID_ID = -1;

class QApplication;
class QPixmap;
class QTimer;
class QString;
class QSize;

class CssHighlighter;
class MainMenu;
class Database_Manifest;
class Database;
class MoneyDelegate;
class TenantDelegate;
class CommentDelegate;

class Preferences;

class Lease_Agreement;
class Maintenance;
class Invoice;
class Landlord;
class Property;
class Property_Feature;
class Property_Room;
class Receipt;
class Rent;
class Tenant;
class Damage;
class Attachment;


enum class EMode : uint8_t;
enum class EDue : uint8_t;
enum class EMonth : uint8_t;
enum class ETitle : uint8_t;
enum class EEntityType : uint8_t;
enum class EPropertyType : uint8_t;
enum class ERoomType : uint8_t;
enum class ECondition : uint8_t;
enum class EEnergy : uint8_t;
enum class EFeatureType : uint8_t;
enum class EStatus : uint8_t;
enum class ESeverity : uint8_t;
enum class ELeaseType : uint8_t;
enum class EResponsibility : uint8_t;


DEFINE_ENUM(ETable, uint8_t,     //
            Attachment, 1,       //
            Damage, 2,           //
            Invoice, 3,          //
            Landlord, 4,         //
            LandlordP, 5,        //
            Lease_Agreement, 6,  //
            Maintenance, 7,      //
            Property, 8,         //
            Property_Feature, 9, //
            Property_Room, 10,   //
            Receipt, 11,         //
            Rent, 12,            //
            Tenant, 13,          //
)

#endif // FORWARD_H