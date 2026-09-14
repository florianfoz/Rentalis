#include "base.h"
#include "database/manager.h"
#include "mainmenu.h"
#include "rentalis_settings.h"

#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QMessageBox>
#include <QStyle>
#include <QStyleFactory>
#include <QToolTip>
#include <QTranslator>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QApplication::setStyle(QStyleFactory::create("Fusion"));
  QApplication::setPalette(QApplication::style()->standardPalette());

  QCoreApplication::setApplicationName("Rentalis");


  if (init_welcome() == QDialog::Rejected) return 0;

  WAIT_DATABASE_TO_START = false;

  init_save_path();

  init_print_path();

  init_traductions();

  init_themes();

  app.setStyleSheet(RentalisSettings::theme_css());

  QTranslator translator;
  if (translator.load(RentalisSettings::locale().name())) {
    QApplication::installTranslator(&translator);
    qDebug() << "Language " << RentalisSettings::locale() << " loaded !";
  }

  Database_Manager::menu = new MainMenu();
  Database_Manager::menu->showMaximized();
  return QApplication::exec();
}
