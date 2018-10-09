#include "rsales.h"
#include "fsale.h"
#include "rlog.h"
#include "estoredoc.h"
#include "global.h"
#include "data.h"
#include "esaleorder.h"

RSales::RSales(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Sales"));

    m_orderId = "";
    m_groupOrderId = true;

    m_dateStart= QDate::currentDate();
    m_dateEnd = QDate::currentDate();
    m_groupDate = false;

    m_fullDateStart = QDateTime::currentDateTime();
    m_fullDateStart.setTime(QTime::fromString("00:00:00"));
    m_fullDateEnd = QDateTime::currentDateTime();
    m_fullDateEnd.setTime(QTime::fromString("23:59:00"));
    m_fullDate = false;

    m_table = "";
    m_tableName = "";
    m_groupTable = false;

    m_waiter = "";
    m_waiterName = "";
    m_groupWaiter = true;

    m_dishGroup = "";
    m_dishGroupName = "";
    m_groupDishGroup = false;

    m_dish = "";
    m_dishName = "";
    m_groupDish = false;

    topToolBar->addAction(QIcon(":icons/deleted.png"), tr("Deleted"), this, SLOT(slotViewLog()));
    topToolBar->addAction(QIcon(":icons/calc.png"), tr("Count output of goods"), this, SLOT(slotCountFoodOutpup()));
    getData();
}

void RSales::getData()
{
    QString select = "SELECT ", where = "", group = "";

    where = QString("WHERE SH.STATUS_ID=2 AND CAST(OM.DATA AS TIMESTAMP) BETWEEN '%1' AND '%2' ").arg(m_fullDateStart.toString("dd.MM.yyyy hh:mm:ss")).arg(m_fullDateEnd.toString("dd.MM.yyyy hh:mm:ss"));
    if (m_groupDate)
    {
        select += "SH.DATE_CASH,";
        group += "SH.DATE_CASH,";
    }

    if (m_fullDate) {
        select += "CAST(OM.DATA AS TIMESTAMP) AS FULL_DATE,";
        group += "OM.DATA,";
    }

    if (m_orderId.length())
        where += "AND SH.ID IN(" + m_orderId + ") ";
    if (m_groupOrderId)
    {
        select += "SH.ID,";
        group += "SH.ID,";
    }

    if (m_table.length())
        where += "SH.TABLE_ID IN (" + m_table + ") ";
    if (m_groupTable)
    {
        select += "HT.NAME AS TABLE_NAME,";
        group += "HT.NAME,";
    }

    if (m_waiter.length())
        where += "SH.WAITER_ID IN (" + m_waiter + ") ";
    if (m_groupWaiter)
    {
        select += "E.NAME AS WAITER_NAME,";
        group += "E.NAME,";
    }


    if (m_dishGroup.length())
        where += "AND D.GROUP_ID IN (" + m_dishGroup + ") ";
    if (m_groupDishGroup)
    {
        select += "DG.NAME AS DGNAME,";
        group += "DG.NAME,";
    }

    if (m_dish.length())
        where += "AND SB.DISH_ID IN (" + m_dish + ") ";
    if (m_groupDish)
    {
        select += "D.NAME AS DNAME,";
        group += "D.NAME,";
    }

    view_order_mode = (!m_dishGroup.length()) && (!m_groupDishGroup) && (!m_dish.length()) && (!m_groupDish);
    if (view_order_mode)
        select += " SUM(SH.AMOUNT) AS AMOUNT "
                "FROM SALE_HEADER SH "
                "LEFT JOIN EMPLOYEES E ON SH.WAITER_ID=E.ID "
                "LEFT JOIN HALL_TABLES HT ON SH.TABLE_ID=HT.ID "
                "LEFT JOIN SALE_MODS OM ON SH.ID=OM.ORDER_ID AND OM.MOD_ID=2 ";
    else
        select += " SUM(SB.QTY) AS QTY, SUM(SB.QTY*SB.PRICE) AS AMOUNT "
                "FROM SALE_HEADER SH "
                "LEFT JOIN EMPLOYEES E ON SH.WAITER_ID=E.ID "
                "LEFT JOIN HALL_TABLES HT ON SH.TABLE_ID=HT.ID "
                "LEFT JOIN SALE_BODY SB ON SH.ID=SB.ORDER_ID "
                "LEFT JOIN DISHES D ON D.ID=SB.DISH_ID "
                "LEFT JOIN DISHES_GROUPS DG ON DG.ID=D.GROUP_ID "
                "LEFT JOIN SALE_MODS OM ON SH.ID=OM.ORDER_ID AND OM.MOD_ID=2 ";

    if (group.length() > 0)
        group = "GROUP BY " + group.remove(group.length() - 1, 1);

    table->ExecuteQuery(select + where + group);

    table->setColumn("ID", tr("Order id"), 120, DATA_STRING);
    table->setColumn("DATE_CASH", tr("Date"), 160, DATA_DATE);
    table->setColumn("FULL_DATE", tr("Full date"), 200, DATA_DATETIME);
    table->setColumn("TABLE_NAME", tr("Table"), 200, DATA_STRING);
    table->setColumn("WAITER_NAME", tr("Waiter"), 200, DATA_STRING);
    table->setColumn("DGNAME", tr("Dishes group"), 200, DATA_STRING);
    table->setColumn("DNAME", tr("Dish"), 200, DATA_STRING);

    if (view_order_mode)
        table->setColumn("AMOUNT", tr("Amount"), 150, DATA_DOUBLE);
    else
    {
        table->setColumn("QTY", tr("Quantity"), 100, DATA_DOUBLE, 2);
        table->setColumn("AMOUNT", tr("Amount"), 150, DATA_DOUBLE, 2);
    }
}

void RSales::slotFilter()
{
    FSale *f = new FSale(this);
    f->m_peOrderId->setText(m_orderId);
    f->m_pchOrderId->setChecked(m_groupOrderId);

    f->m_pd1->setDate(m_dateStart);
    f->m_pd2->setDate(m_dateEnd);
    f->m_pchDate->setChecked(m_groupDate);

    f->m_pfullDate1->setDateTime(m_fullDateStart);
    f->m_pfullDate2->setDateTime(m_fullDateEnd);
    f->m_pchFullDate->setChecked(m_fullDate);

    f->m_pTable->setText(m_tableName);
    f->m_table = m_table;
    f->m_pchTable->setChecked(m_groupTable);

    f->m_pWaiter->setText(m_waiterName);
    f->m_waiter = m_waiter;
    f->m_pchWaiter->setChecked(m_groupWaiter);

    f->m_dishGroup = m_dishGroup;
    f->m_peDishGroup->setText(m_dishGroupName);
    f->m_pchDishGroup->setChecked(m_groupDishGroup);

    f->m_dish = m_dish;
    f->m_peDish->setText(m_dishName);
    f->m_pchDish->setChecked(m_groupDish);

    if (f->exec() == QDialog::Accepted)
    {
        m_orderId = f->m_peOrderId->text();
        m_groupOrderId = f->m_pchOrderId->isChecked();

        m_dateStart = f->m_pfullDate1->date();
        m_dateEnd = f->m_pfullDate2->date();
        m_groupDate = f->m_pchDate->isChecked();

        m_fullDateStart = f->m_pfullDate1->dateTime();
        m_fullDateEnd = f->m_pfullDate2->dateTime();
        m_fullDate = f->m_pchFullDate->isChecked();

        m_tableName = f->m_pTable->text();
        m_table = f->m_table;
        m_groupTable = f->m_pchTable->isChecked();

        m_waiterName = f->m_pWaiter->text();
        m_waiter = f->m_waiter;
        m_groupWaiter = f->m_pchWaiter->isChecked();

        m_dishGroup = f->m_dishGroup;
        m_dishGroupName = f->m_peDishGroup->text();
        m_groupDishGroup = f->m_pchDishGroup->isChecked();

        m_dish = f->m_dish;
        m_dishName = f->m_peDish->text();
        m_groupDish = f->m_pchDish->isChecked();
        getData();
    }
    delete f;
}

void RSales::slotTotal()
{
    double qty = 0, amount = 0;
    for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
    {
        amount += table->m_model.Double("AMOUNT", i);
        if (!view_order_mode)
            qty += table->m_model.Double("QTY", i);
    }

    const QLocale l = QLocale::c();
    QMessageBox::information(this, tr("Total"),
                             QString("<html><head><style type=\"txt/css\">td {padding: 10px;}</style></head>"
                                     "<table>"
                                     "<tr><td>%1</td><td>%2</td></tr>" //total records count
                                     "<tr><td>%3</td><td>%4</td></tr>" //total amount
                                     "<tr><td>%5</td><td>%6</td></tr>" //total quantity of dish
                                     "</table></html>" //total quantity of food
                                     )
                             .arg(tr("Record count"))
                             .arg(l.toString(table->m_model.rowCount(QModelIndex())))
                             .arg(tr("Amount"))
                             .arg(l.toString(amount, 'f', 0))
                             .arg(tr("Dishes quantity"))
                             .arg(l.toString(qty, 'f', 2)));
}

void RSales::slotDelete()
{
    if (!M::user.m_roles.contains(ROLE_DEL_SALE_DOCS))
    {
        QMessageBox::critical(this, tr("Error"), tr("Access denied"));
        return;
    }

    if (!m_groupOrderId)
    {
        QMessageBox::critical(this, tr("Error"), tr("Select 'Group by order' option in the filter"));
        return;
    }

    QModelIndexList l = table->selectionModel()->selectedRows();
    QVector<QString> list;
    for (QModelIndexList::const_iterator i = l.begin(); i != l.end(); i++)
        if (!list.contains(table->m_model.String("ID", i->row())))
            list.push_back(table->m_model.String("ID", i->row()));

    if (list.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    if (QMessageBox::warning(this, tr("Warning"), tr("Are you sure to delete the selected rows?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    QSqlQuery *q = table->openDB();
    for (int i = list.count() - 1; i > -1; i--)
    {
        q->prepare("DELETE FROM SALE_HEADER WHERE ID=:ID");
        q->bindValue(":ID", list[i]);
        q->exec();
        q->prepare("delete from cards_history where orderid=:ORDERID");
        q->bindValue(":ORDERID", list[i]);
        q->exec();
    }
    table->closeDB(q);
    slotRefresh();
}

void RSales::slotPrint()
{
    QMessageBox::information(this, tr("Print"), tr("Not implemented"));
}

void RSales::slotViewLog()
{
    RLog *r = new RLog(this);
    r->setDate(m_dateStart, m_dateEnd);
    r->show();
}

void RSales::slotEdit()
{
    if (!m_groupOrderId)
    {
        QMessageBox::critical(this, tr("Error"), tr("Select 'Group by order' option in the filter"));
        return;
    }

    QVector<int> rows;
    table->getSelectedRows(rows);

    if (!rows.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing is selected"));
        return;
    }

    for (QVector<int>::const_iterator i = rows.begin(); i != rows.end(); i++)
    {
        SaleOrder *s = new SaleOrder(this);
        QSqlQuery *q = table->openDB();
        if (!q->prepare("SELECT SB.DISH_ID, D.NAME, SB.QTY, SB.PRICE, SB.QTY*SB.PRICE "
                        "FROM SALE_BODY SB, DISHES D "
                        "WHERE SB.DISH_ID=D.ID AND SB.ORDER_ID=:ORDER_ID"))
            QMessageBox::critical(this, "SQL error", q->lastError().databaseText());
        q->bindValue("ORDER_ID", table->m_model.String("ORDER_ID", *i));
        q->exec();
        while (q->next())
        {
            int row = s->m_pOrder->rowCount();
            s->m_pOrder->setRowCount(row + 1);

            for (int i = 0; i < s->m_pOrder->columnCount(); i++)
            {
                QTableWidgetItem *item  = new QTableWidgetItem(q->value(i).toString());
                s->m_pOrder->setItem(row, i, item);
            }

        }

        q->prepare("SELECT M.NAME, SM.DATA "
                   "FROM MODIFIERS M, SALE_MODS SM "
                   "WHERE M.ID=SM.MOD_ID AND SM.ORDER_ID=:ORDER_ID");
        q->bindValue("ORDER_ID", table->m_model.String("ORDER_ID", *i));
        q->exec();
        while (q->next()) {
            int row = s->m_pAdditional->rowCount();
            s->m_pAdditional->setRowCount(row + 1);
            for (int i = 0; i < s->m_pAdditional->columnCount(); i++) {
                QTableWidgetItem *ti = new QTableWidgetItem(q->value(i).toString());
                s->m_pAdditional->setItem(row, i, ti);
            }
        }

        table->closeDB(q);
        s->exec();
        delete s;
    }
}

void RSales::slotCountFoodOutpup()
{
    int progress = 0;
    QProgressDialog *p = new QProgressDialog(this);
    p->show();
    p->setWindowModality(Qt::WindowModal);
    p->setCancelButton(0);
    p->setMaximum(100);

    QSqlQuery *q = table->openDB();
    QVector<GoodsOutput> goods;

    p->setLabelText(tr("Delete old result"));
    if (!q->prepare("DELETE FROM SALE_OUTPUT WHERE SALE_ID IN (SELECT SB.ID FROM SALE_BODY SB, SALE_HEADER SH WHERE SB.ORDER_ID=SH.ID AND SH.DATE_CASH BETWEEN :DATE1 AND :DATE2)")) {
        QMessageBox::critical(this, tr("SQL Error"), q->lastError().databaseText());
        delete p;
        return;
    }
    q->bindValue("DATE1", m_dateStart);
    q->bindValue("DATE2", m_dateEnd);
    q->exec();
    progress += 5;
    p->setValue(progress);

    p->setLabelText(tr("Get food output"));
    if(!q->prepare("SELECT SB.ID, R.STORE_ID, SB.DISH_ID, R.GOODS_ID, SB.QTY*R.QTY "
               "FROM SALE_HEADER SH, RECIPES R, SALE_BODY SB "
               "WHERE SH.ID=SB.ORDER_ID AND SB.DISH_ID=R.DISH_ID AND SH.DATE_CASH BETWEEN :DATE1 AND :DATE2")) {
        QMessageBox::critical(this, tr("SQL Error"), q->lastError().databaseText());
        delete p;
        return;
    }
    q->bindValue("DATE1", m_dateStart);
    q->bindValue("DATE2", m_dateEnd);
    q->exec();
    while (q->next())
    {
        GoodsOutput g;
        g.sale_id = q->value(0).toString();
        g.store_id = q->value(1).toInt();
        g.dish_id = q->value(2).toInt();
        g.goods_id = q->value(3).toInt();
        g.qty = q->value(4).toDouble();
        g.price = 0;
        goods.push_back(g);
    }
    progress += 5;
    p->setValue(progress);

    p->setLabelText(tr("Write result"));

    int progress_step = goods.count() / 90;
    int progress_delta = 0;
    for (QVector<GoodsOutput>::const_iterator i = goods.begin(); i != goods.end(); i++)
    {
        QString sql = "INSERT INTO SALE_OUTPUT (SALE_ID, STORE_ID, DISH_ID, GOODS_ID, QTY, PRICE) VALUES "
                "('%1', %2, %3, %4, %5, %6)";
        sql = sql.arg(i->sale_id).arg(i->store_id).arg(i->dish_id).arg(i->goods_id).arg(i->qty).arg(i->price);

        if (!q->exec(sql)) {
            QMessageBox::critical(this, "SQL error", q->lastError().databaseText());
            delete p;
            return;
        }

        progress_delta++;
        if (!(progress_delta % progress_step)) {
            progress ++;
            p->setValue(progress);
            qApp->processEvents();
        }
    }
    progress = 100;
    p->setValue(progress);

    delete p;
}
