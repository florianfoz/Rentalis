#include "widget/w_rent_tool_declaration.h"

#include "base.h"
#include "ui_w_rent_tool_declaration.h"

#include <QDate>
#include <QDesktopServices>
#include <QSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QUrl>

W_Rent_Tool_Declaration::W_Rent_Tool_Declaration(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Rent_Tool_Declaration)
{
  ui->setupUi(this);

  ui->sb_fisc_year->setValue(QDate::currentDate().year());

  refresh();
}

W_Rent_Tool_Declaration::~W_Rent_Tool_Declaration()
{
  delete ui;
}

void W_Rent_Tool_Declaration::refresh()
{
  if (no_refresh) return;
  no_refresh = true;

  ui->dsb_roof_incomes->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);

  int   year        = ui->sb_fisc_year->value();
  float year_income = 0;

  if (auto properties = DB_MANAGER.get_db()->all_records(ETable::properties)) {
    while (properties->next()) {
      auto property_id                = properties->value("property_id").toInt();
      auto [rent, aid, charge, waste] = DB_MANAGER.get_db()->total_year_rent_sum(year - 1, property_id);
      year_income += rent + aid;
    }
  }

  float abattement_rate   = ui->dsb_abattement->value() / 100;
  float income_to_declare = year_income * (1 - abattement_rate);

  ui->le_total_income->setText(ftom(year_income));
  ui->le_total_abattement->setText(ftom(year_income * abattement_rate));
  ui->le_total_declare->setText(ftom(income_to_declare));

  no_refresh = false;
}

void W_Rent_Tool_Declaration::on_sb_fisc_year_valueChanged(int arg1)
{
  refresh();
}


void W_Rent_Tool_Declaration::on_b_microfoncier_web_clicked()
{
  QDesktopServices::openUrl(
      QUrl("https://www.economie.gouv.fr/particuliers/gerer-mon-argent/investir-dans-limmobilier/"
           "tout-savoir-sur-le-deficit-foncier#r-gime-micro-foncier-et-r-gime-r_1"));
}


void W_Rent_Tool_Declaration::on_b_declaration_web_clicked()
{
  QDesktopServices::openUrl(QUrl("https://www.impots.gouv.fr/particulier/je-declare-mes-locations"));
}
