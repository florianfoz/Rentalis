#ifndef W_RENT_H
#define W_RENT_H

#include "entity/rent.h"
#include "forward.h"

#include <QWidget>

enum class EMonth : uint8_t;

namespace Ui
{
class W_Rent;
}

class W_Rent : public QWidget
{
  Q_OBJECT

public:
  explicit W_Rent(QDate date, qsizetype _property_id);
  explicit W_Rent(qsizetype _id = INVALID_ID);
  ~W_Rent();

  Rent rent;
  bool not_created = false;

  void dosent_exist();
  void inject_data();
  void refresh();
  void set_block_signals(bool block);
private slots:
  void enterEvent(QEnterEvent* event) override;

  void leaveEvent(QEvent* event) override;
  void on_b_new_clicked();

  void on_dsb_rent_valueChanged(double arg1);

  void on_dsb_housing_aid_textChanged(const QString& arg1);

  void on_dsb_charges_textChanged(const QString& arg1);

  void on_dsb_house_wastes_textChanged(const QString& arg1);

  void on_cb_tenant_currentIndexChanged(int index);

  void on_te_comment_textChanged();

  void on_b_delete_clicked();

  void on_b_new_lease_agreement_clicked();

private:
  Ui::W_Rent* ui;
};

#endif // W_RENT_H
