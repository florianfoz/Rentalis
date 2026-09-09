#ifndef W_SETTINGS_H
#define W_SETTINGS_H

#include "settings.h"

#include <QStringList>
#include <QWidget>

class QAbstractButton;
class CssHighlighter;

namespace Ui
{
class W_Settings;
}

class W_Settings : public QWidget
{
  Q_OBJECT

public:
  explicit W_Settings(QWidget* parent = nullptr);
  ~W_Settings();
  CssHighlighter* high;


  void update_settings();

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

  void on_b_ask_to_ai_url_help_clicked();

  void on_b_reset_theme_clicked();

  void on_buttonBox_clicked(QAbstractButton* button);

  void on_cb_theme_currentIndexChanged(int index);

  void on_b_theme_folder_clicked();

  void on_b_db_settings_clicked();

private:
  Ui::W_Settings* ui;
};

#endif // W_SETTINGS_H
