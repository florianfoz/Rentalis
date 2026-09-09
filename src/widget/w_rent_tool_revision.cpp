#include "widget/w_rent_tool_revision.h"

#include "base.h"
#include "ui_w_rent_tool_revision.h"

#include <QDesktopServices>
#include <QUrl>

W_Rent_Tool_Revision::W_Rent_Tool_Revision(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Rent_Tool_Revision)
{
  ui->setupUi(this);

  refresh();
}

W_Rent_Tool_Revision::~W_Rent_Tool_Revision()
{
  delete ui;
}

void W_Rent_Tool_Revision::refresh()
{
  if (no_refresh) return;
  no_refresh = true;

  ui->dsb_rent_current->setPrefix(DB_MANAGER.get_db()->get_settings().get_currency().symbol);

  float IRL_last     = ui->dsb_IRL_last->value();
  float IRL_current  = ui->dsb_IRL_current->value();
  float rent_current = ui->dsb_rent_current->value();

  float new_rent = IRL_last != 0.0f ? rent_current * (IRL_current / IRL_last) : 0.0f;

  ui->le_rent_possible->setText(ftom(new_rent));

  no_refresh = false;
}


void W_Rent_Tool_Revision::on_b_revision_web_clicked()
{
  QDesktopServices::openUrl(QUrl("https://www.anil.org/outils/outils-de-calcul/revision-de-loyer/"));
}

void W_Rent_Tool_Revision::on_dsb_IRL_current_valueChanged(double arg1)
{
  refresh();
}


void W_Rent_Tool_Revision::on_dsb_IRL_last_valueChanged(double arg1)
{
  refresh();
}


void W_Rent_Tool_Revision::on_dsb_rent_current_valueChanged(double arg1)
{
  refresh();
}
