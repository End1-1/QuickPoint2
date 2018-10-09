#include "rstore.h"
#include "estoredoc.h"
#include "fstoredoc.h"
#include "fcalcout.h"
#include "global.h"

RStore::RStore(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Store documents"));
    topToolBar->addAction(QIcon(":icons/calc.png"), tr("Make output document"), this, SLOT(slotGetOutputBySales()));

    m_groupId = true;
    m_docState = "";
    m_groupDocState = false;
    m_date1 = QDate::currentDate().addDays((QDate::currentDate().day() - 1) * -1);
    m_date2 = QDate::currentDate();
    m_groupDate = true;
    m_docType = "";
    m_groupDocType = true;
    m_food = "";
    m_groupFood = false;
    m_storeIn = "";
    m_groupStoreIn = false;
    m_storeOut = "";
    m_groupStoreOut = false;

    m_outDate1 = QDate::currentDate().addDays((-1 * QDate::currentDate().day()) + 1).toString("dd.MM.yyyy");
    m_outDate2 = QDate::currentDate().toString("dd.MM.yyyy");
    m_outStoreName = "";
    m_outStoreId = "";

    getData();
}

void RStore::getData()
{
    QString select = "SELECT ", group = "", where = "";

    if (m_groupId)
    {
        select += "SD.ID,";
        group += "SD.ID,";
    }
    if (m_groupDate)
    {
        select += "SD.DOC_DATE,";
        group += "SD.DOC_DATE,";
    }
    if (m_groupDocType)
    {
        select += "SA.NAME AS SANAME,SD.DOC_TYPE,";
        group += "SA.NAME,SD.DOC_TYPE,";
    }
    if (m_groupDocState)
    {
        select += "DS.NAME AS DOCSTATENAME,SD.DOC_STATE,";
        group += "DS.NAME,SD.DOC_STATE,";
    }
    if (m_groupFood)
    {
        select += "G.NAME AS GNAME,SDR.GOODS_ID,SDR.PRICE,";
        group += "G.NAME,SDR.GOODS_ID,SDR.PRICE,";
    }
    if (m_groupStoreIn)
    {
        select += "SIN.NAME AS SINNAME,SD.STORE_IN_ID,";
        group += "SIN.NAME,SD.STORE_IN_ID,";
    }
    if (m_groupStoreOut)
    {
        select += "SOUT.NAME AS SOUTNAME,SD.STORE_OUT_ID,";
        group += "SOUT.NAME,SD.STORE_OUT_ID,";
    }
    if (m_groupId)
    {
        select += "E.NAME AS ENAME,SD.COMMENT,";
        group += "E.NAME,SD.COMMENT,";
    }

    where = QString("WHERE SD.DOC_DATE BETWEEN '%1' AND '%2' ").arg(m_date1.toString("dd.MM.yyyy")).arg(m_date2.toString("dd.MM.yyyy"));
    if (m_docState.length() > 0)
        where += "AND SD.DOC_STATE IN (" + m_docState + ") ";

    if (m_food.length() > 0)
        where += "AND SDR.GOODS_ID IN (" + m_food + ") ";

    if (m_storeIn.length() > 0)
        where += "AND SD.STORE_IN_ID IN (" + m_storeIn + ") ";

    if (m_storeOut.length() > 0)
        where += "AND SD.STORE_OUT_ID IN (" + m_storeOut + ") ";

    if (m_docType.length())
        where += "AND SD.DOC_TYPE IN (" + m_docType + ") ";

    select += "SUM(SDR.QTY) AS QTY,SUM(SDR.QTY*SDR.PRICE) AS AMOUNT "
            "FROM STORE_DOC SD "
            "LEFT JOIN STORE_DRAFT SDR ON SD.ID=SDR.DOC_ID "
            "LEFT JOIN GOODS G ON SDR.GOODS_ID=G.ID "
            "LEFT JOIN DOC_STATE DS ON SD.DOC_STATE=DS.ID "
            "LEFT JOIN STORE_ACTIONS SA ON SD.DOC_TYPE=SA.ID "
            "LEFT JOIN STORAGE SIN ON SD.STORE_IN_ID=SIN.ID "
            "LEFT JOIN STORAGE SOUT ON SD.STORE_OUT_ID=SOUT.ID "
            "LEFT JOIN EMPLOYEES E ON SD.OPERATOR=E.ID ";

    select += where;

    if (group.length() > 0)
        group = "GROUP BY " + group.remove(group.length() - 1, 1);

    select += group;

    table->ExecuteQuery(select);
    table->setColumn("ID", tr("Document id"), 100, DATA_INTEGER);
    table->setColumn("DOC_TYPE", tr("Document type id"), 0, DATA_INTEGER);
    table->setColumn("SANAME", tr("Document type"), 120, DATA_STRING);
    table->setColumn("DOC_DATE", tr("Date"), 120, DATA_STRING);
    table->setColumn("DOC_STATE", tr("Document state id"), 0, DATA_INTEGER);
    table->setColumn("DOCSTATENAME", tr("Document state"), 120, DATA_STRING);
    table->setColumn("STORE_IN_ID", tr("Store in id"), 0, DATA_INTEGER);
    table->setColumn("SINNAME", tr("Store in"), 120, DATA_STRING);
    table->setColumn("STORE_OUT_ID", tr("Store out id"), 0, DATA_INTEGER);
    table->setColumn("SOUTNAME", tr("Store out"), 120, DATA_STRING);
    table->setColumn("COMMENT", tr("Comment"), 300, DATA_STRING);
    table->setColumn("ENAME", tr("Operator"), 200, DATA_STRING);
    table->setColumn("GOODS_ID", tr("Goods id"), 0, DATA_INTEGER);
    table->setColumn("GNAME", tr("Goods"), 250, DATA_STRING);
    table->setColumn("PRICE", tr("Price"), 100, DATA_DOUBLE, 2);
    table->setColumn("QTY", tr("Quantity"), 150, DATA_DOUBLE, 2);
    table->setColumn("AMOUNT", tr("Amount"), 200, DATA_DOUBLE, 2);
}

void RStore::slotNew()
{
    EStoreDoc *e = new EStoreDoc(this);
    e->show();
}

void RStore::slotFilter()
{
    FStoreDoc *f = new FStoreDoc(this);
    f->m_pchkId->setChecked(m_groupId);
    f->m_pchkDate->setChecked(m_groupDate);
    f->m_pDate1->setDate(m_date1);
    f->m_pDate2->setDate(m_date2);
    f->m_peDocType->setText(m_docType);
    f->m_pchkDocType->setChecked(m_groupDocType);
    f->m_peDocState->setText(m_docState);
    f->m_pchkDocState->setChecked(m_groupDocState);
    f->m_peStoreIn->setText(m_storeIn);
    f->m_pchkStoreIn->setChecked(m_groupStoreIn);
    f->m_peStoreOut->setText(m_storeOut);
    f->m_pchkStoreOut->setChecked(m_groupStoreOut);
    f->m_peFood->setText(m_food);
    f->m_pchkFood->setChecked(m_groupFood);
    if (f->exec() == QDialog::Accepted)
    {
        m_groupId = f->m_pchkId->isChecked();
        m_date1 = f->m_pDate1->date();
        m_date2 = f->m_pDate2->date();
        m_groupDate = f->m_pchkDate->isChecked();
        m_docType = f->m_peDocType->text();
        m_groupDocType = f->m_pchkDocType->isChecked();
        m_docState = f->m_peDocState->text();
        m_groupDocState = f->m_pchkDocState->isChecked();
        m_storeIn = f->m_peStoreIn->text();
        m_groupStoreIn = f->m_pchkStoreIn->isChecked();
        m_storeOut = f->m_peStoreOut->text();
        m_groupStoreOut = f->m_pchkStoreOut->isChecked();
        m_food = f->m_peFood->text();
        m_groupFood = f->m_pchkFood->isChecked();
        getData();
    }
    delete f;
}

void RStore::slotDelete()
{
    if (M::user.m_roles.contains(ROLE_DEL_STORE_DOC))
    {
        QMessageBox::critical(this, tr("Error"), tr("Access denied!"));
        return;
    }

    if (!m_groupId)
    {
        QMessageBox::critical(this, tr("Error"), tr("Select grouping by document number option in the filter"));
        return;
    }

    QVector<int> rows;
    QVector<int> docs;

    table->getSelectedRows(rows);
    if (!rows.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    if (QMessageBox::question(this, tr("Warning"), tr("Are you sure to delete the selected documents?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    QSqlQuery *q = table->openDB();
    if (!q)
    {
        QMessageBox::critical(this, tr("Error"), tr("Error in query constructor"));
        return;
    }

    for (QVector<int>::const_iterator i = rows.begin(); i != rows.end(); i++)
        if (!docs.contains(table->m_model.Integer("ID", *i)))
            docs.push_back(table->m_model.Integer("ID", *i));

    for (int i = docs.count() - 1; i > -1; i--)
    {
        QStringList docs_store;
        q->prepare("SELECT DISTINCT(DOC_ID) "
                   "FROM STORE_GOODS "
                   "WHERE RECORD_GUID IN (SELECT RECORD_GUID FROM STORE_GOODS WHERE DOC_ID=:DOC_ID1 AND QTY>0) "
                   "AND DOC_ID <> :DOC_ID2 AND QTY<0");

        q->bindValue("DOC_ID1", docs[i]);
        q->bindValue("DOC_ID2", docs[i]);
        q->exec();
        while (q->next())
            docs_store.push_back(q->value(0).toString());
        if (docs_store.count())
        {
            QString docs_str;
            for (int i = 0; i < docs.count(); i++)
            {
                docs_str += docs_store[i] + ", ";
                if ((i > 0) && (!(i % 5)))
                    docs_str += "\n";
            }
            docs_str = docs_str.remove(docs_str.length() - 1, 1) + ".";
            QMessageBox::critical(this, tr("Error"), tr("The next documents have depending from this document\n") + docs_str);
            table->closeDB(q);
            return;
        }

        q->prepare("DELETE FROM STORE_DOC WHERE ID=:ID");
        q->bindValue("ID", docs[i]);
        q->exec();
    }

    for (int i = rows.count() - 1; i > -1; i--)
        table->m_model.removeRows(rows[i], 1, QModelIndex());
    table->closeDB(q);

}

void RStore::slotEdit()
{
    if (!m_groupId)
    {
        QMessageBox::critical(this, tr("Error"), tr("Select grouping by document number option in the filter"));
        return;
    }

    QVector<int> rows;
    table->getSelectedRows(rows);

    if (!rows.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    if (rows.count() > 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }

    EStoreDoc *e = new EStoreDoc(this);
    e->m_peId->setText(table->m_model.String("ID", rows[0]));
    e->loadDoc();
    e->show();
}

void RStore::slotTotal()
{
    double total_qty = 0, total_amount = 0;
    for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
    {
        total_qty += table->m_model.Double("QTY", i);
        total_amount += table->m_model.Double("AMOUNT", i);
    }

    QString msg = QString("<html><head><style type=\"txt/css\">td {padding: 10px;}</style></head>"
                          "<table>"
                          "<tr><td>%1</td><td>%2</td></tr>" //total records count
                          "<tr><td>%3</td><td>%4</td></tr>" //total quantity
                          "<tr><td>%5</td><td>%6</td></tr>" //total amount
                          "</table></html>") //total quantity of food
            .arg(tr("Count of records"))
            .arg(table->m_model.rowCount(QModelIndex()))
            .arg(tr("Quantity"))
            .arg(total_qty)
            .arg(tr("Amount"))
            .arg(total_amount);

    QMessageBox::information(this, tr("Information"), msg);
}

void RStore::getOutputData(const QDate &date1, const QDate &date2, const QString &store)
{

    QSqlQuery *q = table->openDB();
    if (!q)
    {
        QMessageBox::critical(this, tr("Error"), tr("Error in query constructor"));
        return;
    }

    q->prepare("SELECT G.ID, G.NAME AS GNAME, G.UNIT_ID, U.NAME AS UNAME, SUM(SO.QTY), 0"
               "FROM GOODS G, UNITS U, SALE_OUTPUT SO, SALE_BODY SB, SALE_HEADER SH "
               "WHERE SH.ID=SB.ORDER_ID AND G.ID=SO.GOODS_ID AND U.ID=G.UNIT_ID AND SB.ID=SO.SALE_ID AND SO.STORE_ID=:STORE_ID "
               "AND SH.DATE_CASH BETWEEN :DATE1 AND :DATE2 "
               "GROUP BY 1, 2, 3, 4");
    q->bindValue("STORE_ID", store);
    q->bindValue("DATE1", date1);
    q->bindValue("DATE2", date2);
    q->exec();

    EStoreDoc *e = new EStoreDoc(this);
    e->m_pdDate->setDate(date2);
    e->m_peAction->setText(QString::number(STORE_SALE));
    e->m_peOperator->setText(M::user.Name);
    e->m_peStoreOut->setText(store);
    e->getActionName();
    e->getStoreOutName();
    e->enableStoreInputs();
    bool empty = true;
    while (q->next())
    {
        empty = false;
        int row = e->m_ptGoods->addRow();
        e->m_ptGoods->m_model.setData(row, "ID", q->value(0).toString());
        e->m_ptGoods->m_model.setData(row, "NAME", q->value(1).toString());
        e->m_ptGoods->m_model.setData(row, "UNIT_ID", q->value(2).toString());
        e->m_ptGoods->m_model.setData(row, "UNIT", q->value(3).toString());
        e->m_ptGoods->m_model.setData(row, "QTY", q->value(4).toDouble());
        e->m_ptGoods->m_model.setData(row, "PRICE", q->value(5).toDouble() / q->value(4).toDouble());
        e->m_ptGoods->m_model.setData(row, "AMOUNT", q->value(5).toDouble());
    }

    if (empty)
    {
        delete e;
        QMessageBox::information(this, tr("Information"), tr("Document is empty"));
    }
    else
    {
        e->countAmount();
        e->show();
    }
}

void RStore::slotGetOutputBySales()
{
    FCalcOut *f = new FCalcOut(this);
    f->m_pDate1->setDate(QDate::fromString(m_outDate1, "dd.MM.yyyy"));
    f->m_pDate2->setDate(QDate::fromString(m_outDate2, "dd.MM.yyyy"));
    f->m_pStore->setText(m_outStoreName);
    f->m_store = m_outStoreId;
    if (f->exec() == QDialog::Accepted)
    {
        m_outDate1 = f->m_pDate1->date().toString("dd.MM.yyyy");
        m_outDate2 = f->m_pDate2->date().toString("dd.MM.yyyy");
        m_outStoreName = f->m_pStore->text();
        m_outStoreId = f->m_store;

        QSqlQuery *q = table->openDB();
        if (!q)
        {
            QMessageBox::critical(this, tr("Error"), tr("Error in query constructor"));
            return;
        }

        //Make store list
        QStringList storeList;
        if (m_outStoreId.length())
        {
            storeList = m_outStoreId.split(",");
        }
        else
        {
            if (!q->prepare("SELECT DISTINCT(SO.STORE_ID) FROM SALE_OUTPUT SO, SALE_HEADER SH, SALE_BODY SB "
                       "WHERE SH.ID=SB.ORDER_ID AND SB.ID=SO.SALE_ID AND SH.DATE_CASH BETWEEN :DATE1 AND :DATE2 "))
            {
                QMessageBox::critical(this, tr("Error"), tr("Query prepare error"));
                return;
            }
            q->bindValue("DATE1", QDate::fromString(m_outDate1, "dd.MM.yyyy"));
            q->bindValue("DATE2", QDate::fromString(m_outDate2, "dd.MM.yyyy"));
            if (!q->exec())
            {
                QMessageBox::critical(this, tr("Error"), tr("Query exec error"));
                return;
            }

            while (q->next())
                storeList.append(q->value(0).toString());
        }
        table->closeDB(q);

        //Make documents
        for (QStringList::const_iterator store = storeList.begin(); store != storeList.end(); store++)
            getOutputData(QDate::fromString(m_outDate1, "dd.MM.yyyy"), QDate::fromString(m_outDate2, "dd.MM.yyyy"), *store);

    }
    delete f;
}
