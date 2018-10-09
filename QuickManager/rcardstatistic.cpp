#include "rcardstatistic.h"
#include "filtercardstat.h"

RCardStatistic::RCardStatistic(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Card use by days"));
    fDate1 = QDate::currentDate().toString("dd.MM.yyyy");
    fDate2 = QDate::currentDate().toString("dd.MM.yyyy");
    getData();
}

void RCardStatistic::slotTotal()
{
    if (fGroup) {
        double a = 0, v = 0;
        int q = 0;
        for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
        {
            q += table->m_model.Integer("QTY", i);
            a += table->m_model.Double("AMOUNT", i);
            v += table->m_model.Double("BALANCE", i);
        }

        const QLocale l = QLocale::c();
        QMessageBox::information(this, tr("Total"),
                                 QString("<html><head><style type=\"txt/css\">td {padding: 10px;}</style></head>"
                                         "<table>"
                                         "<tr><td>%1</td><td>%2</td></tr>" //out
                                         "<tr><td>%3</td><td>%4</td></tr>" //out
                                         "<tr><td>%5</td><td>%6</td></tr>" //in
                                         "</table></html>" //total quantity of food
                                         )
                                 .arg(tr("Qty"))
                                 .arg(q)
                                 .arg(tr("Total"))
                                 .arg(l.toString(a, 'f', 0))
                                 .arg(tr("Balance"))
                                 .arg(l.toString(v, 'f', 0)));

    } else {
        double a = 0, v = 0;
        for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
        {
            a += table->m_model.Double("AMOUNT", i);
            v += table->m_model.Double("VAL", i);
        }

        const QLocale l = QLocale::c();
        QMessageBox::information(this, tr("Total"),
                                 QString("<html><head><style type=\"txt/css\">td {padding: 10px;}</style></head>"
                                         "<table>"
                                         "<tr><td>%1</td><td>%2</td></tr>" //out
                                         "<tr><td>%3</td><td>%4</td></tr>" //in
                                         "</table></html>" //total quantity of food
                                         )
                                 .arg(tr("Total"))
                                 .arg(l.toString(a, 'f', 0))
                                 .arg(tr("Balance"))
                                 .arg(l.toString(v, 'f', 0)));
    }
}

void RCardStatistic::slotFilter()
{
    if (FilterCardStat::getFilter(fCard, fDate1, fDate2, fGroup)) {
        getData();
    }
}

void RCardStatistic::getData()
{
    QString select;
    if (fGroup) {
        select = "select ch.cardid, count(ch.orderid) as qty, sum(sh.amount) as amount, sum(ch.val) as balance "
                "from cards_history ch, sale_header sh "
                "where sh.date_cash between '" + fDate1 +"' and '" + fDate2 + "' and ch.orderid=sh.id " + (fCard.isEmpty() ? "" : " and ch.cardid in(" + fCard + ") ") +
                " group by 1 "
                " order by 1 ";
        table->ExecuteQuery(select);
        table->setColumn("CARDID", tr("Card"), 120, DATA_STRING);
        table->setColumn("QTY", tr("Qty"), 120, DATA_STRING);
        table->setColumn("AMOUNT", tr("Amount"), 120, DATA_DOUBLE);
        table->setColumn("BALANCE", tr("Balance"), 120, DATA_DOUBLE);
    } else {
        select = "select ch.cardid, sh.date_cash, ch.orderid, sh.amount, ch.val "
            "from cards_history ch, sale_header sh "
            "where sh.date_cash between '" + fDate1 +"' and '" + fDate2 + "' and ch.orderid=sh.id " + (fCard.isEmpty() ? "" : " and ch.cardid in(" + fCard + ") ") +
            " order by 1, 2";

        table->ExecuteQuery(select);
        table->setColumn("CARDID", tr("Card"), 120, DATA_STRING);
        table->setColumn("DATE_CASH", tr("Date"), 120, DATA_DATE);
        table->setColumn("ORDERID", tr("Order"), 120, DATA_STRING);
        table->setColumn("AMOUNT", tr("Amount"), 120, DATA_DOUBLE);
        table->setColumn("VAL", tr("Value"), 120, DATA_DOUBLE);
    }
}
