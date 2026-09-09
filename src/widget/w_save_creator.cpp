#include "widget/w_save_creator.h"

#include "base.h"
#include "database.h"
#include "database_manager.h"
#include "db_settings.h"
#include "mainmenu.h"
#include "ui_w_save_creator.h"

#include <QDateTime>
#include <QFile>
#include <QPushButton>
#include <QTimer>

W_Save_Creator::W_Save_Creator(W_Save_Manager* manager)
  : manager(manager)
  , ui(new Ui::W_Save_Creator)
{
  ui->setupUi(this);

  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Ok))
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("Ok");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Cancel))
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("Cancel");
  if (auto btn = ui->buttonBox->button(QDialogButtonBox::Apply))
    ui->buttonBox->button(QDialogButtonBox::Apply)->setObjectName("Apply");
  setWindowTitle(tr("New Save"));

  auto update_current_time = [this]() {
    QDate date = QDate::currentDate();
    ui->le_savetime->setText(date.toString("dd/MM/yyyy HH:mm:ss"));
    ui->le_file_name->setText("rentalis_db_" + sanitize_fileName(DB_MANAGER.get_db()->get_settings().get_name()) + "_"
                              + date.toString("dd-MM-yyyy_HH-mm-ss") + ".sqlite");
  };

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, update_current_time);

  timer->start(1000);
  update_current_time();
}

W_Save_Creator::W_Save_Creator(MainMenu* menu)
  : menu(menu)
  , ui(new Ui::W_Save_Creator)
{
  ui->setupUi(this);

  setWindowTitle(tr("New Save"));

  auto update_current_time = [this]() {
    ui->le_savetime->setText(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss"));
  };

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, update_current_time);

  timer->start(1000);
  update_current_time();
}

W_Save_Creator::~W_Save_Creator()
{
  delete ui;
}

void W_Save_Creator::on_buttonBox_accepted()
{
  auto    date      = QDateTime::currentDateTime();
  QString save_name = "rentalis_db_" + sanitize_fileName(DB_MANAGER.get_db()->get_settings().get_name()) + "_"
                      + date.toString("dd-MM-yyyy_HH-mm-ss") + ".sqlite";

  DB_MANAGER.get_db()->save("manual");

  if (manager) manager->refresh();
  if (menu) menu->refresh();
  this->close();
}


void W_Save_Creator::on_buttonBox_rejected()
{
  this->close();
}
