#include "rcardbyuse.h"
#include "fdaterange.h"
#include "rcardstatistic.h"
#include "defines.h"

rcardbyuse::rcardbyuse(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Use of cards"));
    fDate1 = QDate::currentDate();
    fDate2 = QDate::currentDate();
    getData();
   // connect(this, SIGNAL(dc(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));
}

void rcardbyuse::slotFilter()
{
    if (FDateRange::filter(fDate1, fDate2)) {
        getData();
    }
}

void rcardbyuse::slotTotal()
{
    double o = 0, in = 0;
    for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
    {
        o += table->m_model.Double("I", i);
        in += table->m_model.Double("Օ", i);
    }

    const QLocale l = QLocale::c();
    QMessageBox::information(this, tr("Total"),
                             QString("<html><head><style type=\"txt/css\">td {padding: 10px;}</style></head>"
                                     "<table>"
                                     "<tr><td>%1</td><td>%2</td></tr>" //out
                                     "<tr><td>%3</td><td>%4</td></tr>" //in
                                     "</table></html>" //total quantity of food
                                     )
                             .arg(tr("Bill"))
                             .arg(l.toString(in, 'f', 0))
                             .arg(tr("Debt"))
                             .arg(l.toString(o, 'f', 0)));
}

void rcardbyuse::slotPrint()
{

}

void rcardbyuse::doubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    QString id = table->m_model.String(index.row(), 0);
    RCardStatistic *r = new RCardStatistic(fMainWindow);
    fMainWindow->m->addSubWindow(r);
    dynamic_cast<GridWindow *>(r)->setOnToolBar(fMainWindow->m_pToolBar);
    dynamic_cast<GridWindow *>(r)->show();
}

void rcardbyuse::getData()
{

    QString select = QString("select distinct(h.date_cash) AS DATE_CASH, o.amounto AS O, i.amounti AS I \
            from sale_header h \
            left join (select h1.date_cash, sum(ch.val) as amounto from cards_history ch \
                left join sale_header h1 on h1.id=ch.orderid where ch.val < 0 group by 1) o on o.date_cash=h.date_cash \
            left join (select h2.date_cash, sum(ch.val) as amounti from cards_history ch \
                left join sale_header h2 on h2.id=ch.orderid where ch.val > 0 group by 1) i on i.date_cash=h.date_cash \
            where ((o.amounto <> 0) or (i.amounti <>0)) \
            and h.date_cash between '%1' and '%2' ")
            .arg(fDate1.toString("dd.MM.yyyy"))
            .arg(fDate2.toString("dd.MM.yyyy"));

    table->ExecuteQuery(select);
    table->setColumn("DATE_CASH", tr("Date"), 120, DATA_DATE);
    table->setColumn("O", tr("Bill"), 120, DATA_DOUBLE);
    table->setColumn("I", tr("Debt"), 120, DATA_DOUBLE);
}
