#include "widget/entity/w_landlord.h"

#include "base.h"
#include "entity/landlord.h"
#include "ui_w_landlord.h"
#include "widget/entity/w_landlord_creator.h"
#include "widget/entity/w_landlord_manager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>

W_Landlord::W_Landlord(qsizetype id)
  : id(id)
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
  ui->l_type->setText("(" + EEntityType_to_str(landlord.entity_type) + ")");
  ui->le_email->setText(landlord.email);
  ui->le_address->setText(landlord.address);
  ui->le_phone->setText(landlord.phone);
  ui->le_birthday->setText(landlord.birthday.toString("dd/MM/yyyy"));
  if (!EEntityType_is_human(landlord.entity_type)) {
    ui->le_birthday->setHidden(true);
    ui->l_birthday->setHidden(true);
  }
}

W_Landlord::~W_Landlord()
{
  delete ui;
}

void W_Landlord::on_b_edit_clicked()
{
  auto cre = W_Landlord_Creator(id);
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

  (void)landlord.delete_record();
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
