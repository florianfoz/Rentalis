#include "widget/w_welcome.h"

#include "database/manager.h"
#include "ui_w_welcome.h"

W_Welcome::W_Welcome(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::W_Welcome)
{
  ui->setupUi(this);

  connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(&Database_Manager::instance(), &Database_Manager::signal_db_changed, this, &QDialog::accept);
}

W_Welcome::~W_Welcome()
{
  delete ui;
}
