#include "widget/w_landlord.h"

#include "base.h"
#include "image_buffer.h"
#include "landlord.h"
#include "ui_w_landlord.h"
#include "widget/w_landlord_creator.h"
#include "widget/w_landlord_manager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>

W_Landlord::W_Landlord(W_Landlord_Manager* manager, int id)
  : manager(manager)
  , id(id)
  , ui(new Ui::W_Landlord)
{
  ui->setupUi(this);

  setMouseTracking(true);

  refresh();
}

void W_Landlord::refresh()
{
  ui->b_edit->setHidden(true);
  ui->b_delete->setHidden(true);

  Landlord landlord(id);
  ui->l_name->setText(landlord.get_full_name());
  ui->le_email->setText(landlord.get_email());
  ui->le_address->setText(landlord.get_address());
  ui->le_phone->setText(landlord.get_phone());
  ui->l_singing->setPixmap(landlord.get_singing().get_image());
  ui->le_birthday->setText(landlord.get_birthday().toString("dd/MM/yyyy"));

  if (auto pix = landlord.get_icon().get_image(); !pix.isNull())
    ui->l_icon->setPixmap(pix);
  else
    ui->l_icon->setPixmap(QPixmap("://assets/system-users.svg"));

  if (!is_EEntityType_is_human(landlord.get_entity_type())) {
    ui->le_birthday->setHidden(true);
  }
}

W_Landlord::~W_Landlord()
{
  delete ui;
}

void W_Landlord::on_b_edit_clicked()
{
  auto cre = W_Landlord_Creator(manager, id);
  cre.setModal(true);
  cre.exec();
}

void W_Landlord::on_b_delete_clicked()
{
  Landlord landlord(id);
  QString  msg = QObject::tr("Do you really want to delete the landlord [%1] ?\n").arg(landlord.get_full_name());

  auto result = QMessageBox::warning(this, tr("Landlord Deletion"), msg + TXT::WARNING_OPERATION,
                                     QMessageBox::Yes | QMessageBox::Cancel);

  if (result == QMessageBox::Cancel) return;

  landlord.delete_record();

  manager->refresh();
}

void W_Landlord::enterEvent(QEnterEvent* event)
{
  Q_UNUSED(event);
  ui->b_edit->setHidden(false);
  ui->b_delete->setHidden(false);
}

void W_Landlord::leaveEvent(QEvent* event)
{
  Q_UNUSED(event);
  ui->b_edit->setHidden(true);
  ui->b_delete->setHidden(true);
}
