#include "widget/database/w_database_manager.h"

#include "base.h"
#include "database/manager.h"
#include "database/w_database_importation.h"
#include "ui_w_database_manager.h"
#include "widget/database/w_database.h"
#include "widget/database/w_database_creator.h"

#include <QDesktopServices>
#include <QDir>
#include <QUrl>

W_Database_Manager::W_Database_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Database_Manager)
{
  ui->setupUi(this);

  auto& db = Database_Manager::instance();

  connect(&db, &Database_Manager::signal_db_updated, [this](ETable) { refresh(); });
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

  // first dir is database names
  for (const auto& name : dir_list) {
    // second dir is database time (snapshot)
    auto time_list = QDir(SAVE_PATH() + "/" + name);
    time_list.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    for (const auto& time : time_list.entryList()) {
      auto* w = new W_Database(this, SAVE_PATH() + "/" + name + "/" + time);
      saves.push_back(w);
    }
  }


  for (auto& save : saves) {
    ui->tile_view->add_widget(save);
  }
}

void W_Database_Manager::on_b_new_database_clicked()
{
  auto* w = new W_Database_Creator(this);
  w->setModal(true);
  w->exec();
}


void W_Database_Manager::on_b_open_saved_database_folder_clicked()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(SAVE_PATH()));
}

void W_Database_Manager::on_b_import_database_clicked()
{
  auto* w = new W_Database_Importation();
  w->setModal(true);
  w->exec();
}
