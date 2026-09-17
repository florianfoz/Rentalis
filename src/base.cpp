#include "base.h"

#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "preferences.h"
#include "widget/database/w_database_creator.h"
#include "widget/w_first_welcome.h"
#include "widget/w_welcome.h"

#include <QComboBox>
#include <QDialog>
#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QLocale>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QRegularExpression>
#include <QSettings>
#include <QSql>
#include <QSqlDatabase>
#include <QStandardPaths>
#include <QVBoxLayout>

namespace TXT
{
const QString ABOUT = QObject::tr(
    "<b>Rentalis – Complete Property Management</b><br><br>"
    "Manage your rents, properties, tenants, landlords, invoices and receipts.<br>"
    "- Auto invoices and receipts printer<br>"
    "- Data manager (SQL Base)"
    "- Saved database manager<br>"
    "- Auto EMail writer<br>"
    "- Auto prompt generation for desired AI analytic<br>"
    "- Tax calculation and configurable charts<br><br>"
    "Copyright © 2026 Foz Florian<br>"
    "Rentalis is licensed under the GNU Affero General Public License<br>"
    "version 3 or any later version.<br><br>"
    "Some icons used in this software come from "
    "<a href='https://github.com/PapirusDevelopmentTeam/papirus-icon-theme'>Papirus Icons</a>.<br>"
    "This application uses the Qt 6 framework, which is licensed under the LGPL v3.<br>"
    "See <a href='https://www.qt.io/licensing'>Qt6 Software</a> for more information.<br><br>");

const QString COPYRIGHTS_LABEL = QObject::tr("Rentalis - © 2025 Foz Florian - AGPL-3.0");

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

  return {};
}

QString ftod(float val)
{

  return Preferences::locale().toString(val);
}

QString itod(int val)
{

  return Preferences::locale().toString(val);
}

QString ftom(float val)
{
  return Preferences::locale().toCurrencyString(val, Database_Manager::current_manifest()->currency.symbol, 2);
}

QString itom(int val)
{
  return Preferences::locale().toCurrencyString(val, Database_Manager::current_manifest()->currency.symbol);
}


bool EEntityType_is_human(EEntityType entity_type)
{
  switch (entity_type) {
  case EEntityType::Individual:
  case EEntityType::Family:
  case EEntityType::CoTenant:           return true;
  case EEntityType::Company:
  case EEntityType::Organization:
  case EEntityType::TemporaryOccupant:
  case EEntityType::PrivateInstitution:
  case EEntityType::PublicInstitution:
  case EEntityType::PrivateCollective:
  case EEntityType::PublicCollective:
  case EEntityType::PublicServices:
  default:                              return false;
  }
}


int init_welcome()
{
  // no configuration exists, it's the first software start
  if (!QFile::exists(QSettings("Rentalis", "Rentalis").fileName())) {
    auto* w = new W_First_Welcome();
    w->setModal(true);
    return w->exec();
  }

  auto* w = new W_Welcome();
  w->setModal(true);
  return w->exec();
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
  auto name = input.trimmed();

  // unicode -> ASCII
  name = name.normalized(QString::NormalizationForm_D);

  name.remove(QRegularExpression(R"([\u0300-\u036F])"));

  name = name.toLower();
  // replace special char to "_"
  name.replace(QRegularExpression(R"([\\\/:*?"<>|])"), "_");
  // delete control char
  name.remove(QRegularExpression(R"([\x00-\x1f])"));
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

bool copy_dir(const QString& src, const QString& dest)
{
  QDir source(src);

  if (!source.exists()) {
    return false;
  }

  QDir destination(dest);

  if (!destination.exists() && !destination.mkpath(".")) {
    return false;
  }

  const QFileInfoList entries = source.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

  for (const QFileInfo& entry : entries) {
    const QString sourcePath      = entry.absoluteFilePath();
    const QString destinationPath = destination.filePath(entry.fileName());

    if (entry.isDir()) {
      if (!copy_dir(sourcePath, destinationPath)) {
        return false;
      }
    } else {
      QFile file(sourcePath);

      if (!file.copy(destinationPath)) {
        qDebug() << "Copy error:" << file.errorString();
        return false;
      }
    }
  }

  return true;
}
