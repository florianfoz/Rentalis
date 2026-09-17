#include "base.h"
#include "mainmenu.h"
#include "preferences.h"
#include "savemanager.h"

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

  Preferences::load();
  SaveManager::engage_autosave();


  if (init_welcome() == QDialog::Rejected) return 0;

  init_traductions();

  init_themes();

  app.setStyleSheet(Preferences::theme_css());

  QTranslator translator;
  if (translator.load(Preferences::locale().name())) {
    QApplication::installTranslator(&translator);
    qDebug() << "Language " << Preferences::locale() << " loaded !";
  }

  auto* menu = new MainMenu();
  menu->showMaximized();
  return QApplication::exec();
}
