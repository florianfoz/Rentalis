#include "widget/w_landlord.h"

#include "base.h"
#include "entities/landlord.h"
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

  auto landlord = Landlord::read_record(id);
  ui->l_name->setText(landlord.full_name());
  ui->le_email->setText(landlord.email);
  ui->le_address->setText(landlord.address);
  ui->le_phone->setText(landlord.phone);
  // ui->l_singing->setPixmap(landlord.singing.image);
  ui->le_birthday->setText(landlord.birthday.toString("dd/MM/yyyy"));

  // if (auto pix = landlord.icon.image; !pix.isNull())
  //   ui->l_icon->setPixmap(pix);
  // else
  //   ui->l_icon->setPixmap(QPixmap("://assets/system-users.svg"));

  if (!EEntityType_is_human(landlord.entity_type)) {
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
  auto    landlord = Landlord::read_record(id);
  QString msg      = QObject::tr("Do you really want to delete the landlord [%1] ?\n").arg(landlord.full_name());

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
