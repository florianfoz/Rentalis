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

#ifndef W_WELCOME_H
#define W_WELCOME_H

#include <QDialog>

namespace Ui
{
class W_Welcome;
}

class W_Welcome : public QDialog
{
  Q_OBJECT

public:
  explicit W_Welcome(QWidget* parent = nullptr);
  ~W_Welcome();

private slots:
  void on_b_next_clicked();

private:
  Ui::W_Welcome* ui;
};

#endif // W_WELCOME_H
