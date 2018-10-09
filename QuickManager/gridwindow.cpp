#include "gridwindow.h"
#include "tablemodel.h"
#include <QtGui>
#include <QtSql>
#include "../All/econnection.h"

GridWindow::GridWindow(QWidget *parent) :
    QMainWindow(parent), l(QLocale::c())
{
    m_pAction = 0;
    table = new TableView();
    setCentralWidget(table);
    setMinimumSize(900, 700);

    topToolBar = new QToolBar(this);
    topToolBar->addAction(QIcon(":icons/new.png"), tr("New"), this, SLOT(slotNew()));
    topToolBar->addAction(QIcon(":icons/edit.png"), tr("Edit"), this, SLOT(slotEdit()));
    topToolBar->addAction(QIcon(":icons/delete.png"), tr("Delete"), this, SLOT(slotDelete()));
    topToolBar->addAction(QIcon(":icons/filter.png"), tr("Filter"), this, SLOT(slotFilter()));
    topToolBar->addAction(QIcon(":icons/refresh.png"), tr("Refresh"), this, SLOT(slotRefresh()));
    topToolBar->addAction(QIcon(":icons/total.png"), tr("Total"), this, SLOT(slotTotal()));
    topToolBar->addAction(QIcon(":icons/excel.png"), tr("Export to excel"), this, SLOT(slotExportToExcel()));
    topToolBar->addAction(QIcon(":icons/print.png"), tr("Print"), this, SLOT(slotPrint()));
    addToolBar(topToolBar);

    EConnection::setConnectionParams(table->m_db.db, Data::databaseIndex);
    connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));
}

GridWindow::~GridWindow()
{
    if (m_pAction)
        delete m_pAction;
}

void GridWindow::setOnToolBar(QToolBar *toolBar)
{
    m_pAction = toolBar->addAction(windowTitle());
    connect(m_pAction, SIGNAL(triggered()), this, SLOT(slotAction()));
}

bool GridWindow::prepare(QSqlQuery *query, const QString &sqlStr)
{
    if (!query->prepare(sqlStr))
    {
        QMessageBox::critical(this, tr("SQL error"), query->lastError().databaseText());
        return false;
    }
    return true;
}

bool GridWindow::exec(QSqlQuery *query)
{
    if (!query->exec())
    {
        QMessageBox::critical(this, tr("SQL error"), query->lastError().databaseText());
        return false;
    }
    return true;
}

bool GridWindow::exec(QSqlQuery *query, const QString &sqlStr)
{
    if (!query->exec(sqlStr))
    {
        QMessageBox::critical(this, tr("SQL error"), query->lastError().databaseText());
        return false;
    }
    return true;
}

QVector<int> &GridWindow::getSelectedRows()
{
    selectedRows.clear();
    QModelIndexList l = table->selectionModel()->selectedRows();
    for (QModelIndexList::const_iterator i = l.begin(); i != l.end(); i++)
        if (!selectedRows.contains(i->row()))
            selectedRows.push_back(i->row());
    return selectedRows;
}

void GridWindow::setColumnCaption(const QString &fieldName, const QString &caption)
{
    table->m_model.m_columns[table->m_model.m_colName[fieldName]].Caption = caption;
}

void GridWindow::setColumnWidth(const QString &fieldName, const int &width)
{
    table->setColumnWidth(fieldName, width);
}

void GridWindow::setColumnDataType(const QString &fieldName, const int type)
{
    table->m_model.m_columns[table->m_model.m_colName[fieldName]].DataType = type;
}

void GridWindow::setColumn(const QString &fieldName, const QString &caption, const int &width, const int &type)
{
    setColumnCaption(fieldName, caption);
    setColumnWidth(fieldName, width);
    setColumnDataType(fieldName, type);
}

void GridWindow::slotExportToExcel()
{
    QLibrary lib(QDir::currentPath() + "/excel");
    if (!lib.load())
    {
        QMessageBox::critical(this, "Program error", "Could not load library " + QDir::currentPath() + "/excel.dll");
        return;
    }

    Excel_Create excel_create = (Excel_Create)(lib.resolve("Excel_Create"));
    Excel_Show excel_show = (Excel_Show)(lib.resolve("Excel_Show"));
    Workbook_Add workbook_add = (Workbook_Add)(lib.resolve("Workbook_Add"));
    Sheet_Select sheet_select = (Sheet_Select)(lib.resolve("Sheet_Select"));
    Sheet_Set_Cell_Value sheet_set_cell_value = (Sheet_Set_Cell_Value)(lib.resolve("Sheet_Set_Cell_Value"));
    Sheet_Set_Col_Width sheet_set_col_width = (Sheet_Set_Col_Width)(lib.resolve("Sheet_Set_Col_Width"));
    Clear clear = (Clear)(lib.resolve("Clear"));

    int *excel = excel_create();
    int *workbook = workbook_add(excel);
    int *sheet = sheet_select(workbook, 1);

    for (int i = 0; i < table->m_model.columnCount(QModelIndex()); i++)
    {
        sheet_set_cell_value(sheet, i + 1, 1, table->m_model.m_columns[i].Caption.toStdWString().c_str());
        sheet_set_col_width(sheet, i + 1, table->columnWidth(i) / 7);
    }

    for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
        for (int j =  0; j < table->m_model.columnCount(QModelIndex()); j++)
            sheet_set_cell_value(sheet, j + 1, i + 2, table->m_model.data(j, i).toString().toStdWString().c_str());

    excel_show(excel);
    clear(sheet);
    clear(workbook);
    clear(excel);
}

void GridWindow::slotPrint()
{
    QMessageBox::information(this, tr("Information"), tr("Not implemented"));
}

void GridWindow::slotAction()
{
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    activateWindow();
    setFocus();
}

void GridWindow::doubleClicked(const QModelIndex &index)
{

}

void GridWindow::slotNew()
{
}

void GridWindow::slotEdit()
{
}

void GridWindow::slotDelete()
{
    QMessageBox::information(this, tr("Information"), tr("Not implemented"));
}

void GridWindow::slotFilter()
{
    QMessageBox::information(this, tr("Information"), tr("Not implemented"));
}

void GridWindow::slotRefresh()
{
    getData();
}

void GridWindow::slotTotal()
{
    QMessageBox::information(this, tr("Information"), tr("Not implemented"));
}
