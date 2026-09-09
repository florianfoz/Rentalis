#include "widget/w_save.h"

#include "base.h"
#include "database.h"
#include "ui_w_save.h"
#include "widget/w_save_manager.h"

#include <QMessageBox>

W_Save::W_Save(W_Save_Manager* manager, const QString& _file_name)
  : manager(manager)
  , file_name(_file_name)
  , ui(new Ui::W_Save)
{
  ui->setupUi(this);

  refresh();
}

void W_Save::refresh()
{
  Database db;
  db.open_database(file_name);

  if (!db.is_valid()) return;

  ui->l_savetime->setText(db.get_settings().get_save_time().toString("dd/MM/yyyy HH:mm:ss"));
  ui->te_desc->setText(db.get_settings().get_description());
  ui->le_save_mode->setText(db.get_settings().get_save_method());
  ui->le_currency->setText(db.get_settings().get_currency().to_str());
  ui->le_language->setText(db.get_settings().get_language().name());
  ui->le_location->setText(db.get_file_path());
  ui->l_name->setText(db.get_settings().get_name());
}

W_Save::~W_Save()
{
  delete ui;
}

void W_Save::on_b_delete_clicked()
{
  Database db;
  db.open_database(file_name);
  if (!db.is_valid()) return;

  db.remove();
  if (manager) manager->refresh();
}


void W_Save::on_b_load_clicked()
{
  auto db = std::make_unique<Database>();
  if (!db->open_database(file_name)) {
    return;
  }

  DB_MANAGER.change_database(std::move(db));
}
