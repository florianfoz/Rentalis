#include "widget/w_comment_dialog.h"

#include "base.h"
#include "property.h"
#include "tenant.h"
#include "ui_w_comment_dialog.h"
#include "widget/w_rent.h"

#include <QPushButton>

W_Comment_Dialog::W_Comment_Dialog(int rent_id)
  : rent_id(rent_id)
  , rent(Rent(rent_id))
  , ui(new Ui::W_Comment_Dialog)
{
  ui->setupUi(this);

  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");

  setWindowTitle(tr("Comment Rent"));
  resize(400, 300);

  auto property = Property(rent.get_property_id());
  auto tenant   = Tenant(rent.get_tenant_id());

  if (rent.is_loaded() && property.is_loaded() && tenant.is_loaded()) {
    ui->le_year->setText(QString::number(rent.get_date().year()));
    ui->le_month->setText(QString::number(rent.get_date().month()));
    ui->le_property->setText(property.get_name());

    ui->le_rent->setText(ftom(rent.get_rent()));
    ui->le_housing_aid->setText(ftom(rent.get_housing_aid()));
    ui->le_charges->setText(ftom(rent.get_charge()));
    ui->le_other->setText(ftom(rent.get_household_waste()));

    ui->le_tenant->setText(tenant.get_full_name());
    ui->te_comment->setPlainText(rent.get_comment());
  }
}

W_Comment_Dialog::~W_Comment_Dialog()
{
  delete ui;
}

void W_Comment_Dialog::on_buttonBox_accepted()
{
  DB_MANAGER.get_db()->update_record(ETable::rents, rent.get_id(), "comment", ui->te_comment->toPlainText());
}


void W_Comment_Dialog::on_buttonBox_rejected()
{
  this->close();
}
