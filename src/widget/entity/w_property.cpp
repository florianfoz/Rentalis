#include "widget/entity/w_property.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "entity/property.h"
#include "ui_w_property.h"
#include "widget/entity/w_property_creator.h"
#include "widget/entity/w_property_manager.h"

#include <QMessageBox>
#include <QRegularExpression>
#include <QSql>
#include <QSqlQuery>


W_Property::W_Property(qsizetype id)
  : id(id)
  , ui(new Ui::W_Property)
{
  ui->setupUi(this);

  refresh();
}

void W_Property::refresh()
{
  auto property = Property::read_record(id);
  ui->l_name->setText(property.name);
  ui->l_surface_size->setText(ftod(property.surface) + " m³");
  ui->le_address->setText(property.address);
  ui->le_location_number->setText(property.location_number);
  ui->le_type->setText(EPropertyType_to_str(property.property_type));
  ui->le_country->setText(property.country);
  ui->le_city->setText(property.city);
  ui->le_address->setText(property.address);
  ui->le_postal_code->setText(property.postal_code);
  ui->le_condition->setText(ECondition_to_str(property.condition));
  ui->le_energy->setText(EEnergy_to_str(property.energy));
}

W_Property::~W_Property()
{
  delete ui;
}

void W_Property::on_b_edit_clicked()
{
  auto cre = W_Property_Creator(id);
  cre.setModal(true);
  cre.exec();
}


void W_Property::on_b_delete_clicked()
{
  auto property = Property::read_record(id);

  if (Property::is_used(id)) {
    QString msg = QObject::tr(R"(
            "Deleting the property [%1] is prohibied.\n"
            "Because he is referenced in the database (rents) !\n"
            "You should remove all his references, or else change his name to correspond to an desired property."
        )")
                      .arg(property.name);

    QMessageBox::warning(nullptr, tr("Prohibied Property Deletion"), msg);
    return;
  }

  QString msg = QObject::tr(R"(
            "Do you really want to delete the property [%1] ?\n"
        )")
                    .arg(property.name)
                + TXT::WARNING_OPERATION;

  auto result = QMessageBox::warning(this, tr("Property Deletion"), msg, QMessageBox::Yes | QMessageBox::Cancel);

  if (result == QMessageBox::Cancel) return;

  (void)property.delete_record();
}
