#include "rfinalreport.h"
#include "fsaleout.h"
#include "estoredoc.h"

RFinalReport::RFinalReport(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Final report"));
    m_date1 = QDate::currentDate().addDays(-1 * QDate::currentDate().day());
    m_date2 = QDate::currentDate();
    m_docDate = m_date2;
    m_store = "";
    getData();
}

void RFinalReport::getData()
{
    m_store = "";

    FSaleOut *f = new FSaleOut(this);
    f->m_pdDate1->setDate(m_date1);
    f->m_pdDate2->setDate(m_date2);
    f->m_pdDocDate->setDate(m_docDate);
    f->m_peStore->setText(m_store);
    f->slotStoreByCode();
    if (f->exec() == QDialog::Accepted)
    {
        m_date1 = f->m_pdDate1->date();
        m_date2 = f->m_pdDate2->date();
        m_docDate = f->m_pdDocDate->date();
        m_store = f->m_peStore->text();
    }
    delete f;

    if (m_store.length() == 0)
        return;

    m_data.clear();
    QSqlQuery *q = table->openDB();
    /* Initial */
    getSnapshot(*q, m_date1, m_store);
    while (q->next())
    {
        QMap<int, QtyRow>::iterator i = getOneRow(q->value(0).toInt(), q->value(1).toString());
        i->Initial = q->value(2).toDouble();
    }

    /* Incoming */
    getMovement(*q, m_date1, m_date2, STORE_INPUT, true);
    while (q->next())
    {
        QMap<int, QtyRow>::iterator i = getOneRow(q->value(0).toInt(), q->value(1).toString());
        i->Incoming = q->value(2).toDouble();
    }

    /* Movement, in */
    getMovement(*q, m_date1, m_date2, STORE_MOVEMENT, true);
    while (q->next())
    {
        QMap<int, QtyRow>::iterator i = getOneRow(q->value(0).toInt(), q->value(1).toString());
        i->MovementIn = q->value(2).toDouble();
    }

    /* OUT */
    getMovement(*q, m_date1, m_date2, STORE_OUTPUT, false);
    while (q->next())
    {
        QMap<int, QtyRow>::iterator i = getOneRow(q->value(0).toInt(), q->value(1).toString());
        i->Out = q->value(2).toDouble();
    }

    /* Movement, out */
    getMovement(*q, m_date1, m_date2, STORE_MOVEMENT, false);
    while (q->next())
    {
        QMap<int, QtyRow>::iterator i = getOneRow(q->value(0).toInt(), q->value(1).toString());
        i->MovementOut = q->value(2).toDouble();
    }

    /* Sale */
    getMovement(*q, m_date1, m_date2, STORE_SALE, false);
    while (q->next())
    {
        QMap<int, QtyRow>::iterator i = getOneRow(q->value(0).toInt(), q->value(1).toString());
        i->Sale = q->value(2).toDouble();
    }


    /* FACT */
    getSnapshot(*q, m_date1, m_store);
    while (q->next())
    {
        QMap<int, QtyRow>::iterator i = getOneRow(q->value(0).toInt(), q->value(1).toString());
        i->Fact = q->value(2).toDouble();
    }

    for (QMap<int, QtyRow>::iterator i = m_data.begin(); i != m_data.end(); i++)
    {
        i->Calculated = i->Initial + i->Incoming + i->MovementIn + i->Out + i->MovementOut + i->Sale;
        i->Difference = i->Fact - i->Calculated;
    }

    if (m_data.count() == 0)
        return;

    table->m_model.removeRows(0, table->m_model.rowCount(QModelIndex()) - 1, QModelIndex());
    table->m_model.removeColumns(0, table->m_model.columnCount(QModelIndex()) - 1, QModelIndex());
    table->m_model.insertColumns(0, 11, QModelIndex());
    table->m_model.insertRows(0, m_data.count(), QModelIndex());

    int i = -1;
    table->setColumn(++i, "GOODS_ID", tr("Goods code"), 100, DATA_INTEGER);
    table->setColumn(++i, "GOODS_NAME", tr("Goods name"), 200, DATA_STRING);
    table->setColumn(++i, "QTY_TILL", tr("Initial"), 100, DATA_DOUBLE, 1);
    table->setColumn(++i, "QTY_IN", tr("Incoming"), 100, DATA_DOUBLE, 1);
    table->setColumn(++i, "QTY_MOVEMENTIN", tr("Movement, in"), 100, DATA_DOUBLE, 1);
    table->setColumn(++i, "QTY_OUT", tr("Output"), 100, DATA_DOUBLE, 1);
    table->setColumn(++i, "QTY_MOVEMENTOUT", tr("Movement, out"), 100, DATA_DOUBLE, 1);
    table->setColumn(++i, "QTY_SALE", tr("Sale"), 100, DATA_DOUBLE, 1);
    table->setColumn(++i, "QTY_AFTER", tr("Calculated"), 100, DATA_DOUBLE, 1);
    table->setColumn(++i, "QTY_FACT", tr("Fact"), 100, DATA_DOUBLE, 1);
    table->setColumn(++i, "QTY_DIFF", tr("Difference"), 100, DATA_DOUBLE, 1);

    for (int j = 0; j < m_data.count(); j++)
        setData(j, m_data.values().at(j));
}

QMap<int, RFinalReport::QtyRow>::iterator RFinalReport::getOneRow(const int &goods_id, const QString &goods_name)
{
    if (!m_data.contains(goods_id))
    {
        m_data[goods_id] = QtyRow();
        m_data[goods_id].foodCode = goods_id;
        m_data[goods_id].foodName = goods_name;
        m_data[goods_id].Initial = 0;
        m_data[goods_id].Incoming = 0;
        m_data[goods_id].MovementIn = 0;
        m_data[goods_id].Out = 0;
        m_data[goods_id].MovementOut = 0;
        m_data[goods_id].Sale = 0;
        m_data[goods_id].Calculated = 0;
        m_data[goods_id].Fact;
        m_data[goods_id].Difference = 0;
    }
    return m_data.find(goods_id);
}

void RFinalReport::setData(const int &row, const RFinalReport::QtyRow &data)
{
    table->m_model.setData(row, "GOODS_ID", data.foodCode);
    table->m_model.setData(row, "GOODS_NAME", data.foodName);
    table->m_model.setData(row, "QTY_TILL", data.Initial);
    table->m_model.setData(row, "QTY_IN", data.Incoming);
    table->m_model.setData(row, "QTY_MOVEMENTIN", data.MovementIn);
    table->m_model.setData(row, "QTY_OUT", data.Out);
    table->m_model.setData(row, "QTY_MOVEMENTOUT", data.MovementOut);
    table->m_model.setData(row, "QTY_SALE", data.Sale);
    table->m_model.setData(row, "QTY_AFTER", data.Calculated);
    table->m_model.setData(row, "QTY_FACT", data.Fact);
    table->m_model.setData(row, "QTY_DIFF", data.Difference);
}

void RFinalReport::getSnapshot(QSqlQuery &q, QDate &date, QString store)
{
    q.prepare("SELECT SG.GOODS_ID, G.NAME AS GNAME, SUM(SG.QTY) AS QTY, SUM(SG.QTY*SG.PRICE) AS AMOUNT "
               "FROM STORE_GOODS SG, STORE_DOC SD, GOODS G "
               "WHERE SG.DOC_ID=SD.ID AND SG.GOODS_ID=G.ID "
               "AND SD.DOC_DATE=:DATE1 AND SD.DOC_TYPE=:DOC_TYPE AND SG.STORE_ID=:STORE_ID "
               "GROUP BY SG.GOODS_ID, G.NAME");
    q.bindValue("DATE1", date);
    q.bindValue("DOC_TYPE", STORE_INVENTORY);
    q.bindValue("STORE", store);
    q.exec();
}

void RFinalReport::getMovement(QSqlQuery &q, QDate &date1, QDate &date2, int doc_type, bool d)
{
    q.prepare(QString("SELECT SG.GOODS_ID, G.NAME AS GNAME, SUM(SG.QTY) AS QTY, SUM(SG.QTY*SG.PRICE) AS AMOUNT "
               "FROM STORE_GOODS SG, STORE_DOC SD, GOODS G "
               "WHERE SG.DOC_ID=SD.ID AND SG.GOODS_ID=G.ID "
               "AND (SD.DOC_DATE BETWEEN :DATE1+1 AND :DATE2) AND (SD.DOC_TYPE=:DOC_TYPE AND SG.QTY") + (d ? ">" : "<") + QString("0) "
               "AND SG.STORE_ID=:STORE_ID "
               "GROUP BY SG.GOODS_ID, G.NAME "));
    q.bindValue("DATE1", date1);
    q.bindValue("DATE2", date2);
    q.bindValue("DOC_TYPE", doc_type);
    q.bindValue("STORE", m_store);
    q.exec();
}
