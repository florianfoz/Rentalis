#include "widget/w_database.h"

#include "database/database.h"
#include "database/manager.h"
#include "database/manifest.h"
#include "ui_w_database.h"
#include "widget/w_database_manager.h"

#include <QMessageBox>

W_Database::W_Database(W_Database_Manager* manager, const QString& _save_path)
  : manager(manager)
  , save_path(_save_path)
  , ui(new Ui::W_Database)
{
  ui->setupUi(this);

  refresh();
}

void W_Database::refresh()
{
  auto* db = Database::open_database(save_path);

  if (!db->is_valid()) return;

  ui->l_savetime->setText(db->manifest().last_save_time.toString("dd/MM/yyyy HH:mm:ss"));
  ui->te_desc->setText(db->manifest().description);
  ui->le_save_mode->setText(db->manifest().save_method);
  ui->le_currency->setText(db->manifest().currency.to_str());
  ui->le_language->setText(db->manifest().language.name());
  ui->le_location->setText(db->file_path());
  ui->l_name->setText(db->manifest().name);
}

W_Database::~W_Database()
{
  delete ui;
}

void W_Database::on_b_delete_clicked()
{
  auto* db = Database::open_database(save_path);
  if (!db->is_valid()) return;

  db->remove();
  if (manager) manager->refresh();
}


void W_Database::on_b_load_clicked()
{
  Database_Manager::load_database(save_path);
}
