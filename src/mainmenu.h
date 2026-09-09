#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>

class QResizeEvent;
class QPushButton;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainMenu;
}
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
  Q_OBJECT

public:
  MainMenu(QWidget* parent = nullptr);
  ~MainMenu();

  void refresh();

  void open_save_menu();

  QVector<QWidget*>     indicators;
  QVector<QPushButton*> buttons;

private slots:
  void on_a_about_triggered();

  void on_a_save_triggered();

  void on_a_open_saves_file_triggered();

  void on_a_open_sources_file_triggered();

  void on_a_open_prints_file_triggered();

  void on_a_restart_triggered();

  void on_a_quit_triggered();

  void on_a_new_save_triggered();

  void on_a_new_invoice_triggered();

  void on_a_new_receipt_triggered();

  void on_a_new_tenant_triggered();

  void on_a_new_property_triggered();

  void on_a_calc_rent_revision_triggered();

  void on_a_calc_taxes_triggered();

  void on_a_web_declaration_triggered();

  void on_a_web_taxes_triggered();

  void on_a_web_rent_revision_triggered();

  void on_b_saves_clicked();

  void on_b_properties_clicked();

  void on_b_tenants_clicked();

  void on_b_receipts_clicked();

  void on_b_invoices_clicked();

  void on_b_rents_clicked();

  void on_b_source_clicked();

  void on_b_reboot_clicked();

  void on_b_shutdown_clicked();

  void on_b_menu_clicked();

  void on_b_prints_clicked();

  void on_b_newsave_clicked();

  void on_b_landlords_clicked();

  void on_b_saves_file_clicked();

  void on_b_settings_clicked();

  void resizeEvent(QResizeEvent* event) override;

  void on_b_maintenaces_clicked();

  void on_b_damages_clicked();

  void on_a_new_database_triggered();

  void on_a_database_settings_triggered();

  void on_a_welcome_triggered();

  void on_b_sidebar_clicked();

  void on_b_side_panel_clicked();

  void on_b_new_window_clicked();

private:
  Ui::MainMenu* ui;
};
#endif // MAINMENU_H
