#include "widget/w_files_view.h"

#include "ui_w_files_view.h"

W_Files_View::W_Files_View(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Files_View)
{
  ui->setupUi(this);
}

W_Files_View::~W_Files_View()
{
  delete ui;
}
