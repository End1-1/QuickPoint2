#include "rlog.h"
#include <QMessageBox>

RLog::RLog(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Log viewer"));
    m_date1 = QDate::currentDate();
    m_date2 = QDate::currentDate();
}

void RLog::setDate(const QDate date1, const QDate date2)
{
    m_date1 = date1;
    m_date2 = date2;
    getData();
}


void RLog::getData()
{
    QString sql = QString("SELECT L.LOG_DATE, LT.NAME AS LOGNAME, L.DOC_ID, L.DOC_DATE, E.NAME AS ENAME, "
                          "L.LOG_MESSAGE, L.QTY AS QTY, L.PRICE * L.QTY AS AMOUNT "
                          "FROM LOG L "
                          "LEFT JOIN LOG_TYPES LT ON LT.ID=L.LOG_TYPE "
                          "LEFT JOIN EMPLOYEES E ON E.ID=L.USER_ID "
                          "WHERE L.DOC_DATE BETWEEN '%1' AND '%2' ORDER BY L.DOC_DATE ")
            .arg(m_date1.toString("dd.MM.yyyy"))
            .arg(m_date2.toString("dd.MM.yyyy"));
    table->ExecuteQuery(sql);
    table->setColumn("LOG_DATE", tr("Date"), 150, DATA_STRING);
    table->setColumn("LOGNAME", tr("Action"), 300, DATA_STRING);
    table->setColumn("DOC_ID", tr("Document id"), 200, DATA_INTEGER);
    table->setColumn("DOC_DATE", tr("Document date"), 120, DATA_STRING);
    table->setColumn("ENAME", tr("Employee"), 250, DATA_STRING);
    table->setColumn("LOG_MESSAGE", tr("Message"), 400, DATA_STRING);
    table->setColumn("QTY", tr("Quantity"), 120, DATA_DOUBLE);
    table->setColumn("AMOUNT", tr("Amount"), 150, DATA_DOUBLE);
}

void RLog::slotTotal()
{
    double qty = 0, amount = 0;
    for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
    {
        qty += table->m_model.Double("QTY", i);
        amount += table->m_model.Double("AMOUNT", i);
    }

    QMessageBox::information(this, tr("Information"),
                             QString("<html><head><style>td {padding: 10px;}</style></head><body>"
                                     "<table>"
                                     "<tr><td>%1</td><td>%2</td></tr>"
                                     "<tr><td>%3</td><td>%4</td></tr>"
                                     "</table></body></html>")
                             .arg(tr("Total quantity:"))
                             .arg(l.toString(qty, 'f', 2))
                             .arg(tr("Total amount:"))
                             .arg(l.toString(amount, 'f', 2)));
}
