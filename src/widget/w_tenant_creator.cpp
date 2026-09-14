#include "widget/w_tenant_creator.h"

#include "base.h"
#include "entities/tenant.h"
#include "ui_w_tenant_creator.h"
#include "widget/w_tenant_manager.h"

#include <QFileDialog>
#include <QPushButton>

W_Tenant_Creator::W_Tenant_Creator(W_Tenant_Manager* manager, int id)
  : QDialog(manager)
  , manager(manager)
  , tenant(Tenant::read_record(id))
  , ui(new Ui::W_Tenant_Creator)
{
  ui->setupUi(this);

  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto* btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");

  if (id == -1)
    setWindowTitle(tr("Tenant Creation"));
  else
    setWindowTitle(tr("Tenant Edition"));

  populate_ui();
}

W_Tenant_Creator::~W_Tenant_Creator()
{
  delete ui;
}

void W_Tenant_Creator::populate_ui()
{
  clear();

  static auto count = 0;
  for (const auto& title : ETitle_names) {
    ui->cb_title->addItem(title, count++);
  }

  count = 0;
  for (const auto& type : EEntityType_names) {
    ui->cb_type->addItem(type, count++);
  }

  if (tenant) {
    int type_index = ui->cb_type->findData(static_cast<int>(tenant.entity_type));
    if (type_index >= 0) ui->cb_type->setCurrentIndex(type_index);

    int title_index = ui->cb_type->findData(static_cast<int>(tenant.title));
    if (title_index >= 0) ui->cb_type->setCurrentIndex(title_index);

    ui->le_email->setText(tenant.email);
    ui->le_frist_name->setText(tenant.first_name);
    ui->le_last_name->setText(tenant.last_name);
    ui->le_phone->setText(tenant.phone);
  }
}

void W_Tenant_Creator::clear()
{
  ui->cb_title->clear();
  ui->cb_type->clear();
  ui->l_icon->clear();
  ui->le_email->clear();
  ui->le_frist_name->clear();
  ui->le_last_name->clear();
  ui->le_phone->clear();
  ui->de_birthdate->clear();
}

void W_Tenant_Creator::inject_data()
{
  tenant.entity_type = static_cast<EEntityType>(ui->cb_type->currentData().toInt());
  tenant.title       = static_cast<ETitle>(ui->cb_title->currentData().toInt());
  tenant.birthday    = ui->de_birthdate->date();
  tenant.email       = ui->le_email->text();
  tenant.phone       = ui->le_phone->text();
  tenant.first_name  = ui->le_frist_name->text();
  tenant.last_name   = ui->le_last_name->text();
}

void W_Tenant_Creator::on_buttonBox_accepted()
{
  inject_data();

  tenant.save_record();

  if (manager) manager->refresh();
  close();
}


void W_Tenant_Creator::on_buttonBox_rejected()
{
  if (manager) manager->refresh();
  close();
}

void W_Tenant_Creator::on_b_icon_clicked()
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

void W_Tenant_Creator::on_cb_type_currentIndexChanged(int index)
{
  bool is_human = EEntityType_is_human(static_cast<EEntityType>(ui->cb_type->currentData().toInt()));

  ui->l_title->setHidden(!is_human);
  ui->l_birthdate->setHidden(!is_human);
  ui->cb_title->setHidden(!is_human);
  ui->de_birthdate->setHidden(!is_human);
}


void W_Tenant_Creator::on_le_phone_textEdited(const QString& arg1)
{
  static bool lock = false;
  if (lock) return;

  lock = true;

  // Garder uniquement les chiffres
  QString digits;
  for (QChar c : arg1) {
    if (c.isDigit()) digits.append(c);
  }

  // Limiter à 10 chiffres
  if (digits.length() > 10) digits = digits.left(10);

  // Formatter : "99 99 99 99 99"
  QString formatted;
  for (int i = 0; i < digits.length(); ++i) {
    if (i > 0 && i % 2 == 0) formatted.append(' ');
    formatted.append(digits[i]);
  }

  ui->le_phone->setText(formatted);

  // Remettre le curseur à la fin
  ui->le_phone->setCursorPosition(formatted.length());

  lock = false;
}
