#include "widget/w_database_importation.h"

#include "ui_w_database_importation.h"

W_Database_Importation::W_Database_Importation(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::W_Database_Importation)
{
  ui->setupUi(this);
}

W_Database_Importation::~W_Database_Importation()
{
  delete ui;
}

void W_Database_Importation::refresh()
{
}


void W_Database_Importation::on_tb_zip_clicked()
{
}


void W_Database_Importation::on_tb_sqlite_clicked()
{
}


void W_Database_Importation::on_tb_files_clicked()
{
}
