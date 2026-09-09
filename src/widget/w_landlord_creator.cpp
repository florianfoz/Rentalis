#include "widget/w_landlord_creator.h"

#include "base.h"
#include "image_buffer.h"
#include "landlord.h"
#include "ui_w_landlord_creator.h"
#include "widget/w_landlord_manager.h"

#include <QFileDialog>
#include <QPushButton>

W_Landlord_Creator::W_Landlord_Creator(W_Landlord_Manager* manager, int id)
  : QDialog(manager)
  , manager(manager)
  , landlord(new Landlord(id))
  , ui(new Ui::W_Landlord_Creator)
{
  ui->setupUi(this);

  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Apply))
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

  for (auto title : all_titles) {
    ui->cb_title->addItem(ETitle_to_str(title), static_cast<int>(title));
  }

  for (auto type : all_entity_types) {
    ui->cb_type->addItem(EEntityType_to_str(type), static_cast<int>(type));
  }

  if (landlord->is_loaded()) {
    int type_index = ui->cb_type->findData(static_cast<int>(landlord->get_entity_type()));
    if (type_index >= 0) ui->cb_type->setCurrentIndex(type_index);

    int title_index = ui->cb_type->findData(static_cast<int>(landlord->get_title()));
    if (title_index >= 0) ui->cb_type->setCurrentIndex(title_index);

    ui->le_address->setText(landlord->get_address());
    ui->le_email->setText(landlord->get_email());
    ui->le_frist_name->setText(landlord->get_first_name());
    ui->le_last_name->setText(landlord->get_last_name());
    ui->le_phone->setText(landlord->get_phone());
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
  landlord->set_entity_type(int_to_EEntityType(ui->cb_type->currentData().toInt()));
  landlord->set_title(int_to_ETitle(ui->cb_title->currentData().toInt()));
  landlord->set_birthday(ui->de_birthdate->date());
  landlord->set_email(ui->le_email->text());
  landlord->set_phone(ui->le_phone->text());
  landlord->set_first_name(ui->le_frist_name->text());
  landlord->set_last_name(ui->le_last_name->text());

  landlord->set_icon(Image_Buffer(ui->l_icon->pixmap(), icon_type));
  landlord->set_singing(Image_Buffer(ui->l_singing->pixmap(), singing_type));
}

void W_Landlord_Creator::on_buttonBox_accepted()
{
  if (landlord->is_loaded() && landlord->is_dirty()) {
    landlord->update_record();
  } else if (landlord->is_dirty()) {
    landlord->insert_record();
  }

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
