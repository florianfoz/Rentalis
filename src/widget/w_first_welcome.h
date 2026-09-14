/*
 * Copyright (C) 2025 Foz Florian
 *
 * Licensed under the GNU Affero General Public License v3.0
 * or later (AGPL-3.0-or-later).
 *
 * This project uses third-party software, including Qt.
 * Third-party components remain subject to their respective licenses.
 */

#ifndef W_FIRST_WELCOME_H
#define W_FIRST_WELCOME_H

#include <QDialog>

namespace Ui
{
class W_First_Welcome;
}

class W_First_Welcome : public QDialog
{
  Q_OBJECT

public:
  explicit W_First_Welcome(QWidget* parent = nullptr);
  ~W_First_Welcome();

private slots:
  void on_b_next_clicked();

private:
  Ui::W_First_Welcome* ui;
};

#endif // W_FIRST_WELCOME_H
