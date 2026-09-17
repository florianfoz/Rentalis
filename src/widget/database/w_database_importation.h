#ifndef W_DATABASE_IMPORTATION_H
#define W_DATABASE_IMPORTATION_H

#include <QDialog>

namespace Ui
{
class W_Database_Importation;
}

class W_Database_Importation : public QDialog
{
  Q_OBJECT

public:
  explicit W_Database_Importation(QWidget* parent = nullptr);
  ~W_Database_Importation();

  void refresh();
private slots:
  void on_tb_zip_clicked();

  void on_tb_sqlite_clicked();

  void on_tb_files_clicked();

private:
  Ui::W_Database_Importation* ui;
};

#endif // W_DATABASE_IMPORTATION_H
