#include "widget/entity/w_landlord_creator.h"

#include "base.h"
#include "database/manager.h"
#include "entity/landlord.h"
#include "ui_w_landlord_creator.h"

#include <QFileDialog>
#include <QPushButton>

W_Landlord_Creator::W_Landlord_Creator(qsizetype id)
  : QDialog(nullptr)
  , landlord(Landlord::read_record(id))
  , ui(new Ui::W_Landlord_Creator)
{
  ui->setupUi(this);

  if (id == -1)
    setWindowTitle(tr("Landlord Creation"));
  else
    setWindowTitle(tr("Landlord Edition"));

  populate_ui();
}

W_Landlord_Creator::~W_Landlord_Creator()
{
  delete ui;
}

void W_Landlord_Creator::populate_ui()
{
  clear();

  auto count = 0;
  for (const auto& title : ETitle_names) ui->cb_title->addItem(title, count++);
  count = 0;
  for (const auto& type : EEntityType_names) ui->cb_type->addItem(type, count++);


  if (landlord) {
    ui->cb_type->setCurrentIndex(int(landlord.entity_type));

    ui->cb_title->setCurrentIndex(int(landlord.title));

    ui->le_address->setText(landlord.address);
    ui->le_email->setText(landlord.email);
    ui->le_frist_name->setText(landlord.first_name);
    ui->le_last_name->setText(landlord.last_name);
    ui->le_phone->setText(landlord.phone);
  }
}

void W_Landlord_Creator::clear()
{
  ui->cb_title->clear();
  ui->cb_type->clear();
  ui->l_icon->clear();
  ui->l_singing->clear();
  ui->le_address->clear();
  ui->le_email->clear();
  ui->le_frist_name->clear();
  ui->le_last_name->clear();
  ui->le_phone->clear();
  ui->de_birthdate->clear();
}

void W_Landlord_Creator::inject_data()
{
  landlord.entity_type = static_cast<EEntityType>(ui->cb_type->currentData().toInt());
  landlord.title       = static_cast<ETitle>(ui->cb_title->currentData().toInt());
  landlord.birthday    = ui->de_birthdate->date();
  landlord.email       = ui->le_email->text();
  landlord.phone       = ui->le_phone->text();
  landlord.first_name  = ui->le_frist_name->text();
  landlord.last_name   = ui->le_last_name->text();
}

void W_Landlord_Creator::on_buttonBox_accepted()
{
  inject_data();
  (void)landlord.save_record();
  close();
}


void W_Landlord_Creator::on_buttonBox_rejected()
{
  close();
}

void W_Landlord_Creator::on_b_icon_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Select icon image"), "",
                                                  tr("Images (*.png *.jpg *.jpeg *.bmp *.gif);;All files (*)"));

  if (!fileName.isEmpty()) {
    icon_type = QFileInfo(fileName).suffix();

    QPixmap pix(fileName);
    if (!pix.isNull()) {
      ui->l_icon->setPixmap(pix.scaled(ui->l_icon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
      qDebug() << "Impossible to load image :" << fileName;
    }
  }
}


void W_Landlord_Creator::on_b_singing_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Select singature image"), "",
                                                  tr("Images (*.png *.jpg *.jpeg *.bmp *.gif *.svg);;All files (*)"));

  if (!fileName.isEmpty()) {
    singing_type = QFileInfo(fileName).suffix();

    QPixmap pix(fileName);
    if (!pix.isNull()) {
      ui->l_singing->setPixmap(pix.scaled(ui->l_singing->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
      qDebug() << "Impossible to load image :" << fileName;
    }
  }
}
