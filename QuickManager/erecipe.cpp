#include "erecipe.h"
#include "checklist2.h"
#include "astorefood.h"
#include <QApplication>

ERecipe::ERecipe(const QString &defaultStoreId, const QString &defaultStoreName, QWidget *parent) :
    EditWindow(parent), m_defaultStoreId(defaultStoreId), m_defaultStoreName(defaultStoreName)
{
    setWindowTitle(tr("Recipe"));
    int i = -1;
    QGridLayout *l = new QGridLayout();

    QPushButton *pbtn_New = new QPushButton(QIcon(":/icons/new.png"), "", this);
    QPushButton *pbtn_Edit = new QPushButton(QIcon(":/icons/edit.png"), "", this);
    QPushButton *pbtn_Delete = new QPushButton(QIcon(":/icons/delete.png"), "", this);
    QPushButton *pbtn_Excel = new QPushButton(QIcon(":/icons/excel.png"), "", this);
    pbtn_New->setMaximumSize(25, 25);
    pbtn_Edit->setMaximumSize(25, 25);
    pbtn_Delete->setMaximumSize(25, 25);
    pbtn_Excel->setMaximumSize(25, 25);
    connect(pbtn_New, SIGNAL(clicked()), SLOT(slotNew()));
    connect(pbtn_Edit, SIGNAL(clicked()), SLOT(slotEdit()));
    connect(pbtn_Delete, SIGNAL(clicked()), SLOT(slotDelete()));
    connect(pbtn_Excel, SIGNAL(clicked()), SLOT(slotExportToExcel()));

    l->setMargin(0);
    l->addWidget(pbtn_New, ++i, 0);
    l->addWidget(pbtn_Edit, i, 1);
    l->addWidget(pbtn_Delete, i, 2);
    l->addWidget(pbtn_Excel, i, 3);
    l->setColumnStretch(5, 1);

    table = new TableView();
    table->setMinimumSize(750, 400);
    table->m_model.insertColumns(0, 7, QModelIndex());
    table->setColumn(0, "ID", tr("Goods code"), 0, 496);
    table->setColumn(1, "NAME", tr("Goods"), 250, 0);
    table->setColumn(2, "QTY", tr("Qty"), 100, DATA_DOUBLE, 3);
    table->setColumn(3, "UNIT_ID", tr("Unit code"), 0, 496);
    table->setColumn(4, "UNAME", tr("Unit"), 100, 0);
    table->setColumn(5, "STORE_ID", tr("Store id"), 0, DATA_INTEGER);
    table->setColumn(6, "STORE", tr("Store"), 250, DATA_STRING);
    l->addWidget(table, ++i, 0, 1, 4);

    l->addWidget(m_pwidget, ++i, 0, 1, 4);
    setLayout(l);
}

bool ERecipe::checkData()
{
    return true;
}

void ERecipe::setData(AStoreFood *a, const int &row)
{
    table->m_model.setData(row, "ID", a->m_goodsId);
    table->m_model.setData(row, "NAME", a->m_pgoodsName->text());
    table->m_model.setData(row, "UNIT_ID", a->m_unitId);
    table->m_model.setData(row, "UNAME", a->m_pUnit->text());
    table->m_model.setData(row, "STORE_ID", a->m_storeId);
    table->m_model.setData(row, "STORE", a->m_pstoreName->text());
    table->m_model.setData(row, "QTY", a->m_pQty->text());
}

void ERecipe::slotNew()
{
    AStoreFood *a = new AStoreFood(m_defaultStoreId, m_defaultStoreName, this);
    if (a->exec() == QDialog::Accepted)
    {
        int row = table->addRow();
        setData(a, row);
    }
    delete a;
}

void ERecipe::slotEdit()
{
    QModelIndexList rows = table->selectionModel()->selectedRows();
    if (rows.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    AStoreFood *a = new AStoreFood(m_defaultStoreId, m_defaultStoreName, this);
    for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
    {
        a->m_goodsId = table->m_model.String("ID", i);
        a->m_pgoodsName->setText(table->m_model.String("NAME", i));
        a->m_pQty->setText(table->m_model.String("QTY", i));
        a->m_unitId = table->m_model.String("UNIT_ID", i);
        a->m_pUnit->setText(table->m_model.String("UNAME", i));
        a->m_storeId = table->m_model.String("STORE_ID", i);
        a->m_pstoreName->setText(table->m_model.String("STORE", i));

        if (a->exec() == QDialog::Accepted)
            setData(a, i);
    }
    delete a;
}

void ERecipe::slotDelete()
{
    QModelIndexList rows = table->selectionModel()->selectedRows();
    if (rows.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    for (int i = rows.count() - 1; i > -1; i--)
        table->m_model.removeRows(rows[i].row(), 1, QModelIndex());
}

void ERecipe::slotExportToExcel()
{
    QMessageBox::critical(this, "", "");
    QClipboard *c = QApplication::clipboard();
    QString str;

    for (int j = 0; j < table->m_model.rowCount(QModelIndex()); j++) {
        for (int i = 0; i < table->m_model.columnCount(QModelIndex()); i++) {
            str += table->m_model.String(i, j) + "\t";
        }
        str += "\r\n";
    }

    c->setText(str);
}
