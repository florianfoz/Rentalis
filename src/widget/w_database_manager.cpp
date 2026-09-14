#include "widget/w_database_manager.h"

#include "base.h"
#include "database/manager.h"
#include "ui_w_database_manager.h"
#include "widget/w_database.h"
#include "widget/w_database_creator.h"

#include <QDesktopServices>
#include <QDir>
#include <QUrl>

W_Database_Manager::W_Database_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Database_Manager)
{
  ui->setupUi(this);

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&db, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

W_Database_Manager::~W_Database_Manager()
{
  delete ui;
}

void W_Database_Manager::refresh()
{
  ui->tile_view->clear();

  QDir dir(SAVE_PATH());
  dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

  auto               dir_list = dir.entryList();
  QList<W_Database*> saves;
  saves.reserve(dir_list.size());

  for (const auto& dir : dir_list) {
    auto* save = new W_Database(this, SAVE_PATH() + "/" + dir);
    saves.push_back(save);
  }


  for (auto& save : saves) {
    ui->tile_view->add_widget(save);
  }
}

void W_Database_Manager::on_b_new_database_clicked()
{
  auto* save = new W_Database_Creator(this, {});
  save->setModal(true);
  save->exec();
}


void W_Database_Manager::on_b_open_saved_database_folder_clicked()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(SAVE_PATH()));
}


void W_Database_Manager::on_db_loaded()
{
}