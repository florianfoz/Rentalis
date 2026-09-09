#include "widget/w_database_choice.h"

#include "ui_w_database_choice.h"
#include "widget/w_database_creator.h"
#include "widget/w_database_importation.h"

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
  auto w_creator = new W_Database_Creator(nullptr, "");
  w_creator->setModal(true);

  connect(w_creator, &W_Database_Creator::accepted, [this]() { close(); });

  w_creator->exec();
}


void W_Database_Choice::on_pushButton_clicked()
{
  close();
}


void W_Database_Choice::on_b_import_clicked()
{
  auto w_import = new W_Database_Importation();
  w_import->setModal(true);

  connect(w_import, &W_Database_Importation::accepted, [this]() { close(); });

  w_import->exec();
}
