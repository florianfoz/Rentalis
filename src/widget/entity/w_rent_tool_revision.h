#ifndef W_RENT_TOOL_REVISION_H
#define W_RENT_TOOL_REVISION_H

#include <QWidget>

namespace Ui
{
class W_Rent_Tool_Revision;
}

class W_Rent_Tool_Revision : public QWidget
{
  Q_OBJECT

public:
  explicit W_Rent_Tool_Revision(QWidget* parent = nullptr);
  ~W_Rent_Tool_Revision();

  void refresh();

  bool no_refresh = false;
private slots:
  void on_b_revision_web_clicked();
  void on_dsb_IRL_current_valueChanged(double arg1);

  void on_dsb_IRL_last_valueChanged(double arg1);

  void on_dsb_rent_current_valueChanged(double arg1);

private:
  Ui::W_Rent_Tool_Revision* ui;
};

#endif // W_RENT_TOOL_REVISION_H
