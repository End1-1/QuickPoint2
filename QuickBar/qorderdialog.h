#ifndef QORDERDIALOG_H
#define QORDERDIALOG_H

#include "QBaseDialog.h"

class QNumpadWidget;

class QOrderItemsDelegate : public QItemDelegate
{
private:
    QOrder &m_order;

public:
    QOrderItemsDelegate(QOrder &order);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class QMenuItemDelegate : public QItemDelegate
{
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class QInfoItemDelegate : public QItemDelegate
{
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class QOrderDialog : public QBaseDialog
{
    Q_OBJECT
private:
    enum ViewMode {vmDishesGroups = 0, vmDishes, vmMenus};

    QListWidget *m_pMenu;
    QListWidget *m_pOrderItems;
    QListWidget *m_pInfo;
    QNumpadWidget *m_pNumpad;

    int m_menuFilter;
    int m_groupFilter;
    ViewMode m_viewMode;
    QOrder *m_porder;

    QPushButton *m_pbtnServiceCheck;

    void viewOrderDishes(int set_index);
    void viewCommonInfo();
    void printPaymentCheck();
    void closeOrder();
    void moveOrder();
    void recipeOnFly();

public:
    explicit QOrderDialog(QWidget *parent = 0);
    void setTable(const Table &table);
    void setUser(const QUser &user);
    void loadOrder(const QString &order_id);
    bool m_cashMode;
    
signals:
    
public slots:
    void OnSwitchDishes();
    void OnSwitchGroups();
    void OnSwitchMenus();
    void OnMenuClicked(QModelIndex index);
    void OnScrollMenuDown();
    void OnScrollMenuUp();
    void OnNumpadOk();
    void OnScrollOrderDown();
    void OnScrollOrderUp();
    void OnClose();
    void OnDeleteRow();
    void OnServiceCheck();
    void OnOpenMainMenu();
    
};

#endif // QORDERDIALOG_H
