#include "widget/w_property_room_creator.h"

#include "ui_w_property_room_creator.h"

W_Property_Room_Creator::W_Property_Room_Creator(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Property_Room_Creator)
{
  ui->setupUi(this);
}

W_Property_Room_Creator::~W_Property_Room_Creator()
{
  delete ui;
}
