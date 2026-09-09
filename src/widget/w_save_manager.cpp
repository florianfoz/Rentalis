#include "widget/w_save_manager.h"

#include "base.h"
#include "ui_w_save_manager.h"
#include "widget/w_save.h"
#include "widget/w_save_creator.h"

#include <QDesktopServices>
#include <QDir>
#include <QUrl>

W_Save_Manager::W_Save_Manager(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::W_Save_Manager)
{
  ui->setupUi(this);

  connect(&DB_MANAGER, &Database_Manager::signal_db_updated, [this]() { refresh(); });
  connect(&DB_MANAGER, &Database_Manager::signal_db_changed, [this]() { refresh(); });

  refresh();
}

W_Save_Manager::~W_Save_Manager()
{
  delete ui;
}

void W_Save_Manager::refresh()
{
  ui->tile_view->clear();

  QDir dir(SAVE_PATH());
  dir.setFilter(QDir::Files | QDir::NoSymLinks);

  auto                 file_list = dir.entryList();
  std::vector<W_Save*> saves;
  saves.reserve(dir.entryList().size());
  for (const auto& file : file_list) {
    auto save = new W_Save(this, SAVE_PATH() + "/" + file);
    saves.push_back(save);
  }


  for (auto& save : saves) {
    ui->tile_view->add_widget(save);
  }
}

void W_Save_Manager::on_b_new_save_clicked()
{
  auto save = new W_Save_Creator(this);
  save->setModal(true);
  save->exec();
}


void W_Save_Manager::on_b_open_save_folder_clicked()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(SAVE_PATH()));
}
