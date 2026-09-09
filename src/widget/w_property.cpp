#include "widget/w_property.h"

#include "base.h"
#include "property.h"
#include "ui_w_property.h"
#include "widget/w_property_creator.h"
#include "widget/w_property_manager.h"

#include <QMessageBox>
#include <QRegularExpression>
#include <QSql>
#include <QSqlQuery>


W_Property::W_Property(W_Property_Manager* manager, int id)
  : manager(manager)
  , id(id)
  , ui(new Ui::W_Property)
{
  ui->setupUi(this);

  refresh();
}

void W_Property::refresh()
{
  Property property(id);
  ui->l_name->setText(property.get_name());
  ui->l_surface_size->setText(ftod(property.get_surface()) + " m³");
  ui->le_address->setText(property.get_address());
  ui->le_location_number->setText(property.get_location_number());
}

W_Property::~W_Property()
{
  delete ui;
}

void W_Property::on_b_edit_clicked()
{
  auto cre = W_Property_Creator(manager, id);
  cre.setModal(true);
  cre.exec();
}


void W_Property::on_b_delete_clicked()
{
  if (DB_MANAGER.get_db()->property_used(id)) {
    QString msg = QObject::tr(R"(
            "Deleting the property [%1] is prohibied.\n"
            "Because he is referenced in the database (rents) !\n"
            "You should remove all his references, or else change his name to correspond to an desired property."
        )")
                      .arg(Property(id).get_name());

    QMessageBox::warning(nullptr, tr("Prohibied Property Deletion"), msg);
    return;
  } else {
    QString msg = QObject::tr(R"(
            "Do you really want to delete the property [%1] ?\n"
        )")
                      .arg(Property(id).get_name())
                  + TXT::WARNING_OPERATION;

    auto result = QMessageBox::warning(this, tr("Property Deletion"), msg, QMessageBox::Yes | QMessageBox::Cancel);

    if (result == QMessageBox::Cancel) return;

    Property property(id);
    property.delete_record();

    manager->refresh();
  }
}
