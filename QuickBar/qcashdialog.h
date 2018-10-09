#ifndef QCASHDIALOG_H
#define QCASHDIALOG_H

#include "qbasedialog.h"

class QCashDialog : public QBaseDialog
{
    Q_OBJECT

    struct OrderData {
        QString id;
        QString TableName;
        QDateTime ClosedTime;
        QString WaiterName;
        double Amount;
    };

    class QOrdersItemDelegate : public QItemDelegate
    {
    private:
        const QVector<OrderData> &m_data;

    public:
        QOrdersItemDelegate(const QVector<OrderData> &data);

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    };

public:
    explicit QCashDialog(QWidget *parent = 0);
    
private:
    QListWidget *m_pOrders;
    QLineEdit *m_pTotal;
    QVector<OrderData> m_Data;
    void getData();
    
private slots:
    void OnListClicked(QModelIndex index);
    void OnUp();
    void OnDown();
    
};

#endif // QCASHDIALOG_H
