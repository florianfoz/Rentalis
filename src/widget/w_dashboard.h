/*
 * Rentalis
 * Copyright (C) 2026 Foz Florian
 *
 * Rentalis is licensed under the GNU Affero General Public License
 * version 3 or any later version.
 *
 * This project uses third-party software, including Qt.
 * Third-party components remain subject to their respective licenses.
 */

#ifndef W_DASHBOARD_H
#define W_DASHBOARD_H

#include <QWidget>

namespace Ui
{
class W_Dashboard;
}

class W_Dashboard : public QWidget
{
  Q_OBJECT

public:
  explicit W_Dashboard(QWidget* parent = nullptr);
  ~W_Dashboard();

  void refresh();

  bool no_refresh = false;
  void refresh_tw_year_incomes();
  void refresh_tw_tenants();
  void refresh_todo();

private:
  Ui::W_Dashboard* ui;
};

#endif // W_DASHBOARD_H
