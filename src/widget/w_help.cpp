#include "widget/w_help.h"

#include "base.h"
#include "preferences.h"
#include "ui_w_help.h"

#include <QLocale>

W_Help::W_Help(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::W_Help)
{
  ui->setupUi(this);

  {
    QFile f(get_doc_path_localized("README.md"));

    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
      const QString content = QString::fromUtf8(f.readAll());
      ui->README->setText(content);
      f.close();
    }
  }

  {
    QFile f(get_doc_path_localized("NOTICE"));

    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
      const QString content = QString::fromUtf8(f.readAll());
      ui->NOTICE->setText(content);
      f.close();
    }
  }

  {
    QFile f(get_doc_path_localized("LICENSE"));

    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
      const QString content = QString::fromUtf8(f.readAll());
      ui->LICENSE->setText(content);
      f.close();
    }
  }
}

W_Help::~W_Help()
{
  delete ui;
}
