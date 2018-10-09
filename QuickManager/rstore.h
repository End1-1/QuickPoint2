#ifndef RSTORE_H
#define RSTORE_H

#include "gridwindow.h"

class RStore : public GridWindow
{
    Q_OBJECT

public:
    RStore(QWidget *parent = 0);

private:
    void getData();
    void slotNew();

    bool m_groupId;
    QString m_docType;
    bool m_groupDocType;
    QString m_docState;
    bool m_groupDocState;
    QDate m_date1;
    QDate m_date2;
    bool m_groupDate;
    QString m_food;
    bool m_groupFood;
    QString m_storeIn;
    bool m_groupStoreIn;
    QString m_storeOut;
    bool m_groupStoreOut;

    QString m_outDate1;
    QString m_outDate2;
    QString m_outStoreName;
    QString m_outStoreId;

    void slotFilter();
    void slotDelete();
    void slotEdit();
    void slotTotal();
    void getOutputData(const QDate &date1, const QDate &date2, const QString &store);

private slots:
    void slotGetOutputBySales();
};

#endif // RSTORE_H
