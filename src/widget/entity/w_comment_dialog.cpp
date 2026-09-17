#include "widget/entity/w_comment_dialog.h"

#include "base.h"
#include "database/database.h"
#include "database/manager.h"
#include "entity/property.h"
#include "entity/tenant.h"
#include "ui_w_comment_dialog.h"
#include "widget/entity/w_rent.h"

#include <QPushButton>

W_Comment_Dialog::W_Comment_Dialog(int rent_id)
  : rent_id(rent_id)
  , rent(Rent::read_record(rent_id))
  , ui(new Ui::W_Comment_Dialog)
{
  ui->setupUi(this);

  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");

  setWindowTitle(tr("Comment Rent"));
  resize(400, 300);

  auto property = Property::read_record(rent.property_id);
  auto tenant   = Tenant::read_record(rent.tenant_id);

  if (rent && property && tenant) {
    ui->le_year->setText(QString::number(rent.date.year()));
    ui->le_month->setText(QString::number(rent.date.month()));
    ui->le_property->setText(property.name);

    ui->le_rent->setText(ftom(rent.rent));
    ui->le_housing_aid->setText(ftom(rent.housing_aid));
    ui->le_charges->setText(ftom(rent.charge));
    ui->le_other->setText(ftom(rent.household_waste));

    ui->le_tenant->setText(tenant.get_full_name());
    ui->te_comment->setPlainText(rent.comment);
  }
}

W_Comment_Dialog::~W_Comment_Dialog()
{
  delete ui;
}

void W_Comment_Dialog::on_buttonBox_accepted()
{
  (void)Database_Manager::current_recorder()->save_record(ETable::Rent, rent.id, "comment",
                                                          ui->te_comment->toPlainText());
}


void W_Comment_Dialog::on_buttonBox_rejected()
{
  this->close();
}
