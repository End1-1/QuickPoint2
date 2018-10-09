#ifndef RFINALREPORT_H
#define RFINALREPORT_H

#include "gridwindow.h"

class RFinalReport : public GridWindow
{    
    Q_OBJECT

private:
    struct QtyRow {
        int foodCode;
        QString foodName;
        double Initial;
        double Incoming;
        double MovementIn;
        double Out;
        double MovementOut;
        double Sale;
        double Calculated;
        double Fact;
        double Difference;
    };

    QMap<int, QtyRow> m_data;

public:
    RFinalReport(QWidget *parent = 0);

private:
    QDate m_date1;
    QDate m_date2;
    QDate m_docDate;
    QString m_store;
    void getData();
    QMap<int, QtyRow>::iterator getOneRow(const int &goods_id, const QString &goods_name);
    void setData(const int &row, const QtyRow &data);
    void getSnapshot(QSqlQuery &q, QDate &date, QString store);
    void getMovement(QSqlQuery &q, QDate &date1, QDate &date2, int doc_type, bool d);
};

#endif // RFINALREPORT_H
