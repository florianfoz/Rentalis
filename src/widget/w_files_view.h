#ifndef W_FILES_VIEW_H
#define W_FILES_VIEW_H

#include <QWidget>

namespace Ui
{
class W_Files_View;
}

class W_Files_View : public QWidget
{
  Q_OBJECT

public:
  explicit W_Files_View(QWidget* parent = nullptr);
  ~W_Files_View();

private:
  Ui::W_Files_View* ui;
};

#endif // W_FILES_VIEW_H
