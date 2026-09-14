#include "widget/w_landlord_creator.h"

#include "base.h"
#include "entities/landlord.h"
#include "ui_w_landlord_creator.h"
#include "widget/w_landlord_manager.h"

#include <QFileDialog>
#include <QPushButton>

W_Landlord_Creator::W_Landlord_Creator(W_Landlord_Manager* manager, int id)
  : QDialog(manager)
  , manager(manager)
  , landlord(Landlord::read_record(id))
  , ui(new Ui::W_Landlord_Creator)
{
  ui->setupUi(this);

  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");

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
    int type_index = ui->cb_type->findData(static_cast<int>(landlord.entity_type));
    if (type_index >= 0) ui->cb_type->setCurrentIndex(type_index);

    int title_index = ui->cb_type->findData(static_cast<int>(landlord.title));
    if (title_index >= 0) ui->cb_type->setCurrentIndex(title_index);

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
  landlord.save_record();

  if (manager) manager->refresh();
  close();
}


void W_Landlord_Creator::on_buttonBox_rejected()
{
  if (manager) manager->refresh();
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
