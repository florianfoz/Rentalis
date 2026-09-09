#ifndef W_DATABASE_CHOICE_H
#define W_DATABASE_CHOICE_H

#include <QDialog>

namespace Ui
{
class W_Database_Choice;
}

class W_Database_Choice : public QDialog
{
  Q_OBJECT

public:
  explicit W_Database_Choice(QWidget* parent = nullptr);
  ~W_Database_Choice();

private slots:
  void on_b_new_clicked();

  void on_pushButton_clicked();

  void on_b_import_clicked();

private:
  Ui::W_Database_Choice* ui;
};

#endif // W_DATABASE_CHOICE_H
