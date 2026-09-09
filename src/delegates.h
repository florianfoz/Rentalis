#pragma once
#include "base.h"
#include "property.h"
#include "widget/w_comment_dialog.h"
#include "widget/w_rent.h"

#include <QApplication>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QtSql>

// ===================== MoneyDelegate =====================
class MoneyDelegate : public QStyledItemDelegate
{
public:
  MoneyDelegate(QObject* parent = nullptr)
    : QStyledItemDelegate(parent)
  {
  }

  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const override
  {
    QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
    editor->setMinimum(0.0);
    editor->setMaximum(1000000.0);
    editor->setDecimals(2);
    editor->setSuffix(" €");
    return editor;
  }

  void setEditorData(QWidget* editor, const QModelIndex& index) const override
  {
    double value = index.data(Qt::EditRole).toDouble();
    static_cast<QDoubleSpinBox*>(editor)->setValue(value);
  }

  void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override
  {
    double value = static_cast<QDoubleSpinBox*>(editor)->value();
    model->setData(index, value, Qt::EditRole);
  }

  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
  {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    double value = index.data(Qt::EditRole).toDouble(); // Lecture directe de EditRole
    opt.text     = ftom(value);
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
  }
};

// ===================== TenantDelegate =====================
class TenantDelegate : public QStyledItemDelegate
{
public:
  TenantDelegate(QObject* parent = nullptr)
    : QStyledItemDelegate(parent)
  {
  }

  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const override
  {
    QComboBox* editor = new QComboBox(parent);
    auto       f      = editor->font();
    f.setPointSize(10);
    editor->setFont(f);

    QSqlQuery query(DB_MANAGER.get_sql_db());
    query.prepare("SELECT * FROM tenants;");
    if (query.exec()) {
      while (query.next()) {
        editor->addItem(query.value("name").toString(), query.value("tenant_id").toInt());
      }
    }

    return editor;
  }

  void setEditorData(QWidget* editor, const QModelIndex& index) const override
  {
    QString    currentTenant = index.data(Qt::EditRole).toString();
    QComboBox* combo         = static_cast<QComboBox*>(editor);
    int        idx           = combo->findText(currentTenant);
    if (idx >= 0) combo->setCurrentIndex(idx);
  }

  void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override
  {
    QComboBox* combo = static_cast<QComboBox*>(editor);
    model->setData(index, combo->currentText(), Qt::EditRole);
  }

  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
  {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    QString value = index.data(Qt::EditRole).toString();
    opt.text      = value;
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
  }
};

// ===================== CommentDelegate =====================
class CommentDelegate : public QStyledItemDelegate
{
public:
  CommentDelegate(W_Rent* parentWidget)
    : QStyledItemDelegate(parentWidget)
    , rent(parentWidget)
  {
  }

  W_Rent* rent;

  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const override
  {
    Q_UNUSED(parent);

    QString currentText = index.data(Qt::EditRole).toString();
    auto rent_inst = Rent(-1).load_from_property(rent->property->get_id(), QDate(rent->year, rent->col_month + 1, 1));
    W_Comment_Dialog dialog(rent_inst.get_id());
    dialog.setModal(true);
    dialog.exec(); // ouvre la boîte de dialogue

    return nullptr; // pas d'éditeur inline
  }

  void setEditorData(QWidget*, const QModelIndex&) const override
  {
  }
  void setModelData(QWidget*, QAbstractItemModel*, const QModelIndex&) const override
  {
  }
  void updateEditorGeometry(QWidget*, const QStyleOptionViewItem&, const QModelIndex&) const override
  {
  }

  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
  {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    QString value = index.data(Qt::EditRole).toString();
    opt.text      = value;
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
  }
};
