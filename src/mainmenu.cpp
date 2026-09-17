#include "mainmenu.h"

#include "base.h"
#include "database/manager.h"
#include "database/w_database_manager.h"
#include "ui_mainmenu.h"
#include "widget/database/w_database_creator.h"
#include "widget/entity/w_invoice_creator.h"
#include "widget/entity/w_property_creator.h"
#include "widget/entity/w_receipt_creator.h"
#include "widget/entity/w_rent_tool_declaration.h"
#include "widget/entity/w_rent_tool_revision.h"
#include "widget/entity/w_tenant_creator.h"
#include "widget/w_first_welcome.h"
#include "widget/w_preferences.h"
#include "widget/w_welcome.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QUrl>


MainMenu::MainMenu(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainMenu)
{
  ui->setupUi(this);

  indicators = {
      ui->l_menu,       ui->l_rents,     ui->l_invoices,  ui->l_receipts,     ui->l_tenants,
      ui->l_properties, ui->l_databases, ui->l_landlords, ui->l_maintenances, ui->l_damages,
  };

  buttons = {
      ui->b_menu,       ui->b_rents,     ui->b_invoices,    ui->b_receipts, ui->b_tenants,  ui->b_properties,
      ui->b_databases,  ui->b_landlords, ui->b_maintenaces, ui->b_damages,  ui->b_prints,   ui->b_source,
      ui->b_saves_file, ui->b_source,    ui->b_settings,    ui->b_reboot,   ui->b_shutdown,
  };

  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->stackedWidget->setCurrentIndex(0);

  auto* credit_l = new QLabel(TXT::COPYRIGHTS_LABEL);
  auto  f        = credit_l->font();
  f.setPointSize(8);
  credit_l->setFont(f);
  ui->statusBar->addWidget(credit_l);
  setWindowTitle(tr("Dashboard - Rentalis"));

  QPalette pal    = ui->f_side_panel->palette();
  QColor   darker = pal.color(QPalette::Window).darker(125);
  pal.setColor(QPalette::Window, darker);
  ui->f_side_panel->setAutoFillBackground(true);
  ui->f_top_panel->setAutoFillBackground(true);
  ui->statusBar->setAutoFillBackground(true);
  ui->f_side_panel->setPalette(pal);
  ui->f_top_panel->setPalette(pal);
  ui->statusBar->setPalette(pal);

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable table) { refresh(); });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();

  ui->l_menu->show();
}

MainMenu::~MainMenu()
{
  delete ui;
}

void MainMenu::refresh()
{
  /*
      ui->w_menu->refresh();
      ui->w_properties->refresh();
      ui->w_invoices->refresh();
      ui->w_receipts->refresh();
      ui->w_rents->refresh();
      ui->w_saves->refresh();
      ui->w_landlords->refresh();
      ui->w_tenants->refresh();
      */
}

void MainMenu::open_database_menu()
{
  ui->b_databases->click();
}


void MainMenu::on_a_about_triggered()
{
  auto* msg = new QMessageBox();
  msg->setWindowTitle(tr("About Rentalis Software"));
  msg->setText(TXT::ABOUT);
  QPixmap pix("://assets/logo/rentalis.svg");
  pix = pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  msg->setIconPixmap(pix);
  msg->exec();
}

void MainMenu::on_a_save_triggered()
{
  auto cre = W_Database_Creator();
  cre.setModal(true);
  cre.exec();
}

void MainMenu::on_a_open_saves_file_triggered()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(SAVE_PATH()));
}

void MainMenu::on_a_open_sources_file_triggered()
{
  auto f = QFileInfo(DATABASE_PATH());
  QDesktopServices::openUrl(QUrl::fromLocalFile(f.absolutePath()));
}

void MainMenu::on_a_open_prints_file_triggered()
{
  auto f = QFileInfo(PRINT_PATH());
  QDesktopServices::openUrl(QUrl::fromLocalFile(f.absolutePath()));
}

void MainMenu::on_a_restart_triggered()
{
  QString program = QCoreApplication::applicationFilePath();
  QProcess::startDetached(program, QStringList());
  QCoreApplication::quit();
}


void MainMenu::on_a_quit_triggered()
{
  QCoreApplication::quit();
}


void MainMenu::on_a_new_database_triggered()
{
  auto cre = W_Database_Creator();
  cre.setModal(true);
  cre.exec();
}


void MainMenu::on_a_new_invoice_triggered()
{
  auto cre = W_Invoice_Creator();
  cre.setModal(true);
  cre.exec();
}


void MainMenu::on_a_new_receipt_triggered()
{
  auto cre = W_Receipt_Creator();
  cre.setModal(true);
  cre.exec();
}


void MainMenu::on_a_new_tenant_triggered()
{
  auto cre = W_Tenant_Creator();
  cre.setModal(true);
  cre.exec();
}


void MainMenu::on_a_new_property_triggered()
{
  auto cre = W_Property_Creator();
  cre.setModal(true);
  cre.exec();
}


void MainMenu::on_a_web_declaration_triggered()
{
  QDesktopServices::openUrl(QUrl("https://www.impots.gouv.fr/particulier/je-declare-mes-locations"));
}


void MainMenu::on_a_web_taxes_triggered()
{
  QDesktopServices::openUrl(
      QUrl("https://www.economie.gouv.fr/particuliers/gerer-mon-argent/investir-dans-limmobilier/"
           "tout-savoir-sur-le-deficit-foncier#r-gime-micro-foncier-et-r-gime-r_1"));
}


void MainMenu::on_a_web_rent_revision_triggered()
{
  QDesktopServices::openUrl(QUrl("https://www.anil.org/outils/outils-de-calcul/revision-de-loyer/"));
}


void MainMenu::on_a_calc_rent_revision_triggered()
{
  auto* w = new W_Rent_Tool_Revision();
  w->setWindowTitle(tr("Rent Revision"));
  w->setWindowFlags(Qt::Tool);
  w->show();
  refresh();
}


void MainMenu::on_a_calc_taxes_triggered()
{
  auto* w = new W_Rent_Tool_Declaration();
  w->setWindowTitle(tr("Tax Regime"));
  w->setWindowFlags(Qt::Tool);
  w->show();
  refresh();
}


void MainMenu::on_b_databases_clicked()
{
  ui->stackedWidget->setCurrentIndex(7);
  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->l_databases->show();
  setWindowTitle(tr("Databases - Rentalis"));
  refresh();
}


void MainMenu::on_b_properties_clicked()
{
  ui->stackedWidget->setCurrentIndex(5);
  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->l_properties->show();
  setWindowTitle(tr("Properties - Rentalis"));
  refresh();
}


void MainMenu::on_b_tenants_clicked()
{
  ui->stackedWidget->setCurrentIndex(4);
  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->l_tenants->show();
  setWindowTitle(tr("Tenants - Rentalis"));
  refresh();
}


void MainMenu::on_b_receipts_clicked()
{
  ui->stackedWidget->setCurrentIndex(3);
  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->l_receipts->show();
  setWindowTitle(tr("Receipts - Rentalis"));
  refresh();
}


void MainMenu::on_b_invoices_clicked()
{
  ui->stackedWidget->setCurrentIndex(2);
  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->l_invoices->show();
  setWindowTitle(tr("Invoices - Rentalis"));
  refresh();
}


void MainMenu::on_b_rents_clicked()
{
  ui->stackedWidget->setCurrentIndex(1);
  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->l_rents->show();
  setWindowTitle(tr("Rents - Rentalis"));
  refresh();
}

void MainMenu::on_b_landlords_clicked()
{
  ui->stackedWidget->setCurrentIndex(6);
  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->l_landlords->show();
  setWindowTitle(tr("Landlords - Rentalis"));
  refresh();
}


void MainMenu::on_b_menu_clicked()
{
  ui->stackedWidget->setCurrentIndex(0);
  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->l_menu->show();
  setWindowTitle(tr("Dashboard - Rentalis"));
  refresh();
}


void MainMenu::on_b_maintenaces_clicked()
{
  ui->stackedWidget->setCurrentIndex(8);
  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->l_maintenances->show();
  setWindowTitle(tr("Maintenances - Rentalis"));
  refresh();
}


void MainMenu::on_b_damages_clicked()
{
  ui->stackedWidget->setCurrentIndex(9);
  for (auto& elem : indicators) {
    elem->hide();
  }
  ui->l_damages->show();
  setWindowTitle(tr("Damages - Rentalis"));
  refresh();
}


void MainMenu::on_b_source_clicked()
{
  auto f = QFileInfo(DATABASE_PATH());
  QDesktopServices::openUrl(QUrl::fromLocalFile(f.absolutePath()));
}


void MainMenu::on_b_reboot_clicked()
{
  QString program = QCoreApplication::applicationFilePath();
  QProcess::startDetached(program, QStringList());
  QCoreApplication::quit();
}


void MainMenu::on_b_shutdown_clicked()
{
  QCoreApplication::quit();
}

void MainMenu::on_b_prints_clicked()
{
  auto f = QFileInfo(PRINT_PATH());
  QDesktopServices::openUrl(QUrl::fromLocalFile(f.absolutePath()));
}

void MainMenu::on_b_saves_file_clicked()
{
  auto f = QFileInfo(SAVE_PATH());
  QDesktopServices::openUrl(QUrl::fromLocalFile(f.absolutePath()));
}


void MainMenu::on_b_settings_clicked()
{
  static W_Preferences* w = nullptr;
  if (w) w->close();

  w = new W_Preferences();
  w->show();

  connect(w, &QObject::destroyed, [&] { w = nullptr; });
}


void MainMenu::resizeEvent(QResizeEvent* event)
{
  QMainWindow::resizeEvent(event);

  int    h = event->size().height();
  double size;

  if (h < 300) {
    size = 0.0;
  } else {
    size = 24 + (h - 500) * (44 - 24) / (950 - 500);
    size = std::min(size, 44.0);
  }

  for (auto& btn : buttons) {
    btn->setIconSize(QSize(size, size));
  }
}


void MainMenu::on_a_database_settings_triggered()
{
  auto* w = new W_Database_Creator(nullptr, DATABASE_PATH());
  w->setWindowTitle(tr("Database Settings"));
  w->setModal(false);
  w->exec();
}


void MainMenu::on_a_welcome_triggered()
{
  auto* w = new W_Welcome();
  w->exec();
}

void MainMenu::on_a_first_welcome_triggered()
{
  auto* w = new W_First_Welcome();
  w->exec();
}


void MainMenu::on_b_sidebar_clicked()
{
  auto* msg = new QMessageBox();
  msg->setWindowTitle(tr("About Rentalis Software"));
  msg->setText(TXT::ABOUT);
  QPixmap pix("://assets/logo/rentalis.svg");
  pix = pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  msg->setIconPixmap(pix);
  msg->exec();
}


void MainMenu::on_b_side_panel_clicked()
{


  // Animation
  auto* anim = new QPropertyAnimation(ui->f_side_panel, "maximumWidth");
  anim->setDuration(250);
  anim->setEasingCurve(QEasingCurve::OutCubic);
  int start = ui->f_side_panel->maximumWidth();
  int end   = (start == 0) ? 250 : 0;

  QPixmap open("://assets/sidebar-collapse-left.svg");
  QPixmap close("://assets/view-sidetree.svg");
  ui->b_side_panel->setIcon((start == 0) ? open : close);
  ui->b_side_panel->setIconSize(QSize(32, 32));
  ui->b_side_panel->setMinimumSize(QSize(32, 32));

  anim->stop();
  anim->setStartValue(start);
  anim->setEndValue(end);
  anim->start();
}


void MainMenu::on_b_new_window_clicked()
{
  int      page_index = ui->stackedWidget->currentIndex();
  QWidget* w          = nullptr;
  switch (page_index) {
  case 0:  w = new W_Dashboard(nullptr); break;
  case 5:  w = new W_Property_Manager(nullptr); break;
  case 1:  w = new W_Rent_Manager(nullptr); break;
  case 8:  w = new W_Maintenance_Manager(nullptr); break;
  case 9:  w = new W_Damage_Manager(nullptr); break;
  case 6:  w = new W_Landlord_Manager(nullptr); break;
  case 2:  w = new W_Invoice_Manager(nullptr); break;
  case 3:  w = new W_Receipt_Manager(nullptr); break;
  case 4:  w = new W_Tenant_Manager(nullptr); break;
  case 7:  w = new W_Database_Manager(nullptr); break;
  default: break;
  }

  if (!w) return;

  w->setWindowTitle(this->windowTitle());
  w->setAttribute(Qt::WA_DeleteOnClose);
  w->show();
}


void MainMenu::on_a_preferences_triggered()
{
  auto* w = new W_Preferences(this);
  w->show();
}