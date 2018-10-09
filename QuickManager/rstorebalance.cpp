#include "rstorebalance.h"
#include "fstorebalance.h"

RStoreBalance::RStoreBalance(QWidget *parent) :
    GridWindow(parent)
{
    setWindowTitle(tr("Store balance"));
    m_date = QDate::currentDate();
    m_storeNames = "";
    m_storeIDs = "";
    m_groupStore = true;
    m_goodsGroupsNames = "";
    m_goodsGroupsIDs = "";
    m_groupGoodsGroups = true;
    m_goodsNames = "";
    m_goodsIDs = "";
    m_groupGoods = true;
    getData();
}

void RStoreBalance::getData()
{
    // Storage, Goods group, Goods, Quantity, Measurment, Amount
    QString sql = "SELECT ",
            from = "FROM STORE_GOODS SG ",
            join = "LEFT JOIN STORE_DOC SD ON SG.DOC_ID=SD.ID ",
            group,
            where = QString("WHERE SD.DOC_DATE <='%1' ").arg(m_date.toString("dd.MM.yyyy"));

    if (m_groupStore)
    {
        sql += "S.NAME AS STORE_NAME,";
        group += "S.NAME,";
        join += "LEFT JOIN STORAGE S ON SG.STORE_ID=S.ID ";
    }

    if (m_groupGoodsGroups)
    {
        sql += "GG.NAME AS GOODS_GROUP,";
        group += "GG.NAME,";
        join += "LEFT JOIN GOODS G ON SG.GOODS_ID=G.ID ";
        join += "LEFT JOIN GOODS_GROUPS GG ON G.GROUP_ID=GG.ID ";
    }

    if (m_groupGoods)
    {
        sql += "G.NAME AS GOODS_NAME,";
        group += "G.NAME,";
        if (!m_groupGoodsGroups)
            join += "LEFT JOIN GOODS G ON SG.GOODS_ID=G.ID ";
    }

    if (m_storeIDs.length())
        where += "AND SG.STORE_ID IN (" + m_storeIDs + ")";

    if (m_goodsGroupsIDs.length())
    {
        where += "AND G.GROUP_ID IN (" + m_goodsGroupsIDs + ")";
        if ((!m_groupGoods) && (!m_groupGoodsGroups))
            join += "LEFT JOIN GOODS G ON SG.GOODS_ID=G.ID ";
    }

    if (m_goodsIDs.length())
        where += "AND SG.GOODS_ID IN (" + m_goodsIDs + ")";

    sql += "SUM(SG.QTY) AS QTY, SUM(SG.QTY*SG.PRICE) AS AMOUNT ";

    if (group.length())
        group = "GROUP BY " + group.remove(group.length() - 1, 1);

    sql = sql + from + join + where + group + " HAVING SUM(SG.QTY) > 0";
    table->ExecuteQuery(sql);

    table->setColumn("STORE_NAME", tr("Store"), 250, DATA_STRING);
    table->setColumn("GOODS_GROUP", tr("Goods group"), 200, DATA_STRING);
    table->setColumn("GOODS_NAME", tr("Goods"), 300, DATA_STRING);
    table->setColumn("QTY", tr("Quantity"), 150, DATA_DOUBLE, 3);
    table->setColumn("AMOUNT", tr("Amount"), 150, DATA_DOUBLE);

}

void RStoreBalance::slotFilter()
{
    FStoreBalance *f = new FStoreBalance(this);
    f->m_pdate->setDate(m_date);
    f->m_pStoreNames->setText(m_storeNames);
    f->m_StoreIDs = m_storeIDs;
    f->m_pGroupStore->setChecked(m_groupStore);
    f->m_pGroupGoodsNames->setText(m_goodsGroupsNames);
    f->m_GroupGoodsIDs = m_goodsGroupsIDs;
    f->m_pGroupGoodsGroup->setChecked(m_groupGoodsGroups);
    f->m_pGoodsNames->setText(m_goodsNames);
    f->m_GoodsIDs = m_goodsIDs;
    f->m_pGroupGoods->setChecked(m_groupGoods);
    if (f->exec() == QDialog::Accepted)
    {
        m_date = f->m_pdate->date();
        m_storeNames = f->m_pStoreNames->text();
        m_storeIDs = f->m_StoreIDs;
        m_groupStore = f->m_pGroupStore->isChecked();
        m_goodsGroupsNames = f->m_pGroupGoodsNames->text();
        m_goodsGroupsIDs = f->m_GroupGoodsIDs;
        m_groupGoodsGroups = f->m_pGroupGoodsGroup->isChecked();
        m_goodsNames = f->m_pGoodsNames->text();
        m_goodsIDs = f->m_GoodsIDs;
        m_groupGoods = f->m_pGroupGoods->isChecked();
        getData();
    }
    delete f;
}

void RStoreBalance::slotTotal()
{
    double qty = 0, amount = 0;
    for (int i =  0; i < table->m_model.rowCount(QModelIndex()); i++) {
        qty += table->m_model.Double("QTY", i);
        amount += table->m_model.Double("AMOUNT", i);
    }

    QMessageBox::information(this, tr("Total"), QString("<html><body><table><tr><td style=\"width: 250px;\">%1</td><td>%2</td></tr><tr><td>%3</td><td>%4</td></tr></table></body></html>")
                             .arg(tr("Quantity"))
                             .arg(qty)
                             .arg(tr("Amount"))
                             .arg(amount));
}
