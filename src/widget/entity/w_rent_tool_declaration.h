#ifndef W_RENT_TOOL_DECLARATION_H
#define W_RENT_TOOL_DECLARATION_H

#include <QWidget>

namespace Ui
{
class W_Rent_Tool_Declaration;
}

class W_Rent_Tool_Declaration : public QWidget
{
  Q_OBJECT

public:
  explicit W_Rent_Tool_Declaration(QWidget* parent = nullptr);
  ~W_Rent_Tool_Declaration();

  bool no_refresh = false;

  void refresh();
private slots:
  void on_b_declaration_web_clicked();

  void on_sb_fisc_year_valueChanged(int arg1);

  void on_b_microfoncier_web_clicked();

private:
  Ui::W_Rent_Tool_Declaration* ui;
};

#endif // W_RENT_TOOL_DECLARATION_H
