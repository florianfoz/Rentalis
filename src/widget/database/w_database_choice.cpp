#include "widget/database/w_database_choice.h"

#include "ui_w_database_choice.h"
#include "widget/database/w_database_creator.h"
#include "widget/database/w_database_importation.h"

W_Database_Choice::W_Database_Choice(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::W_Database_Choice)
{
  ui->setupUi(this);
}

W_Database_Choice::~W_Database_Choice()
{
  delete ui;
}

void W_Database_Choice::on_b_new_clicked()
{
  auto* w = new W_Database_Creator();
  w->setModal(true);

  connect(w, &W_Database_Creator::accepted, [this]() { close(); });

  w->exec();
}


void W_Database_Choice::on_pushButton_clicked()
{
  close();
}


void W_Database_Choice::on_b_import_clicked()
{
  auto* w = new W_Database_Importation();
  w->setModal(true);

  connect(w, &W_Database_Importation::accepted, [this]() { close(); });

  w->exec();
}
