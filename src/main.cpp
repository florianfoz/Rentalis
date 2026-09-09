#include "base.h"
#include "mainmenu.h"
#include "settings.h"

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
  QApplication a(argc, argv);
  a.setStyle(QStyleFactory::create("Fusion"));
  a.setPalette(QApplication::style()->standardPalette());

  QCoreApplication::setApplicationName("Rentalis");

  APP = &a;

  init_settings();

  if (!init_database()) {
    QMessageBox::critical(nullptr, QObject::tr("No Database"),
                          QObject::tr("No database to load.\nThe software shutdown."));
    return 1;
  }


  init_save_path();

  init_print_path();

  init_traductions();

  init_themes();

  a.setStyleSheet(SETTINGS.get_theme_css());

  QTranslator   translator;
  const QLocale locale = SETTINGS.get_locale();
  if (translator.load(locale.name())) {
    a.installTranslator(&translator);
    qDebug() << "Language " << locale << " loaded !";
  }

  MainMenu w;
  DB_MANAGER.set_mainmenu(&w);
  w.showMaximized();
  return a.exec();
}
