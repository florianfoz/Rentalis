#ifndef W_DATABASE_H
#define W_DATABASE_H

#include <QDateTime>
#include <QWidget>

namespace Ui
{
class W_Database;
}

class W_Database : public QWidget
{
  Q_OBJECT

public:
  explicit W_Database(QWidget* parent = nullptr, const QString& _save_path = {});
  ~W_Database();

  QString save_path;

  bool no_refresh = false;
  void refresh();
private slots:

  void on_b_delete_clicked();
  void on_b_load_clicked();
  void on_b_export_clicked();
  void on_b_edit_clicked();

private:
  Ui::W_Database* ui;
};

#endif // W_DATABASE_H
