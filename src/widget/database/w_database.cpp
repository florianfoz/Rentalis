#include "widget/database/w_database.h"

#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "savemanager.h"
#include "ui_w_database.h"
#include "widget/database/w_database_creator.h"

#include <QFileDialog>
#include <QMessageBox>

W_Database::W_Database(QWidget* parent, const QString& _save_path)
  : QWidget(parent)
  , save_path(_save_path)
  , ui(new Ui::W_Database)
{
  ui->setupUi(this);

  refresh();
}

void W_Database::refresh()
{
  auto manifest = Database_Manifest::load(save_path + "/manifest.conf");

  if (!manifest.is_valid()) return;

  ui->l_savetime->setText(manifest.last_save_time.toString("dd/MM/yyyy HH:mm:ss"));
  ui->te_desc->setText(manifest.description);
  ui->le_save_mode->setText(manifest.save_method);
  ui->le_currency->setText(manifest.currency.to_str());
  ui->le_language->setText(manifest.language.name());
  ui->le_location->setText(save_path);
  ui->l_name->setText(manifest.name);
}

W_Database::~W_Database()
{
  delete ui;
}

void W_Database::on_b_delete_clicked()
{
  SaveManager::delete_save(save_path);
}


void W_Database::on_b_load_clicked()
{
  SaveManager::load_save(save_path);
}

void W_Database::on_b_export_clicked()
{
  QString dest = QFileDialog::getExistingDirectory(this, tr("Select a folder to save the database"), QString(),
                                                   QFileDialog::ShowDirsOnly);

  SaveManager::export_save(save_path, dest);
}

void W_Database::on_b_edit_clicked()
{
  auto* w = new W_Database_Creator(this, save_path);
  w->setModal(true);
  w->open();
}
