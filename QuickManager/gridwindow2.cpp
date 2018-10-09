#include "gridwindow2.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include "global.h"
#include "../All/econnection.h"

int GridWindow2::m_dbNumber;

GridWindow2::GridWindow2(QString title, QWidget *parent) :
    QMainWindow(parent)
{
    setMinimumSize(600, 500);
    setWindowTitle(title);
    m_pToolBar = new QToolBar(this);
    addToolBar(m_pToolBar);

    m_pToolBar->addAction(QIcon(":icons/new.png"), tr("New"), this, SLOT(slotNew()));
    m_pToolBar->addAction(QIcon(":icons/edit.png"), tr("Edit"), this, SLOT(slotEdit()));
    m_pToolBar->addAction(QIcon(":icons/delete.png"), tr("Delete"), this, SLOT(slotDelete()));
    m_pToolBar->addAction(QIcon(":icons/filter.png"), tr("Filter"), this, SLOT(slotFilter()));
    m_pToolBar->addAction(QIcon(":icons/refresh.png"), tr("Refresh"), this, SLOT(slotRefresh()));
    m_pToolBar->addAction(QIcon(":icons/excel.png"), tr("Export to excel"), this, SLOT(slotExportToExcel()));

    m_pTable = new QTableWidget(this);
    setCentralWidget(m_pTable);

    m_currentDbNumber = QString::number(++m_dbNumber);
    m_db = QSqlDatabase::addDatabase("QIBASE", m_currentDbNumber);
    EConnection::setConnectionParams(m_db, Data::databaseIndex);
}

GridWindow2::~GridWindow2()
{
    QSqlDatabase::removeDatabase(m_currentDbNumber);
    if (m_pAction)
        delete m_pAction;
}

void GridWindow2::setOnToolBar(QToolBar *toolBar)
{
    m_pAction = toolBar->addAction(windowTitle());
    connect(m_pAction, SIGNAL(triggered()), this, SLOT(slotAction()));
}

void GridWindow2::initDatabase(QString dbPath, QString username, QString password)
{
    m_db.setDatabaseName(dbPath);
    m_db.setUserName(username);
    m_db.setPassword(password);
}

bool GridWindow2::query(QSqlQuery &q, const QString &tables, const QStringList &columns, const QStringList &where, QStringList &order)
{
    QString sql = "SELECT ";

    if (!columns.count())
        sql += "* ";
    else {
        for (QStringList::const_iterator i = columns.begin(); i != columns.end(); i++)
            sql += *i + ",";
        sql = sql.remove(sql.length() - 1, 1) + " ";
    }

    sql += "FROM " + tables + " ";

    if (where.count())  {
        sql += "WHERE ";
        for (QStringList::const_iterator i = where.begin(); i != where.end(); i++)
            sql += *i + " AND ";
        sql = sql.remove(sql.length() - 4, 4) + " ";
    }

    if (order.count()) {
        sql += "ORDER BY ";
        for (QStringList::const_iterator i = order.begin(); i != order.end(); i++)
            sql += *i + ",";
        sql = sql.remove(sql.length() - 1, 1);
    }

    if (!q.prepare(sql)) {
        QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
        return false;
    }

    if (!q.exec()) {
        QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
        return false;
    }

    return true;
}

int GridWindow2::insert(const QString &genId, const QString &table, QMap<QString, QVariant> &values)
{
    m_db.open();
    QSqlQuery q(m_db);

    int result = 0;
    if (genId.length())
    {
        if (!q.exec("SELECT GEN_ID(" + genId + ", 1) FROM RDB$DATABASE")) {
            m_db.close();
            QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
            return -1;
        }
        q.next();
        result = q.value(0).toInt();
        values["ID"] = result;
    }

    QString sql = "INSERT INTO " + table + " (";
    for (QMap<QString, QVariant>::const_iterator i = values.begin(); i != values.end(); i++)
        sql += i.key() + ",";
    sql = sql.remove(sql.length() - 1, 1) + ") VALUES (";
    for (QMap<QString, QVariant>::const_iterator i = values.begin(); i != values.end(); i++)
        sql += ":" + i.key() + ",";
    sql = sql.remove(sql.length() - 1, 1) + ");";

    if (!q.prepare(sql)) {
        m_db.close();
        QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
        return -1;
    }

    for (QMap<QString, QVariant>::const_iterator i = values.begin(); i != values.end(); i++)
        q.bindValue(i.key(), i.value());

    if (!q.exec()) {
        m_db.close();
        QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
        return -1;
    }

    m_db.close();
    return result;
}

bool GridWindow2::update(const QString &table, const QMap<QString, QVariant> &values, const QMap<QString, QVariant> &where)
{
    m_db.open();
    QSqlQuery q(m_db);
    QString sql = "UPDATE " + table + " SET ";
    for (QMap<QString, QVariant>::const_iterator i = values.begin(); i != values.end(); i++)
        sql += i.key() + "=:" + i.key() + ",";
    sql = sql.remove(sql.length() - 1, 1) + " ";

    if (where.count()) {
        QString strWhere;
        for (QMap<QString, QVariant>::const_iterator i = where.begin(); i != where.end(); i++)
            strWhere += " AND " + i.key() + "=:" + i.key();
        sql += "WHERE " + strWhere.remove(0, 4);
    }

    if (!q.prepare(sql)) {
        QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
        return false;
    }

    for (QMap<QString, QVariant>::const_iterator i = values.begin(); i != values.end(); i++)
        q.bindValue(":" + i.key(), i.value());

    if (where.count()) {
        for (QMap<QString, QVariant>::const_iterator i = where.begin(); i != where.end(); i++)
            q.bindValue(":" + i.key(), i.value());
    }

    if (!q.exec()) {
        QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
        return false;
    }

    sql = q.lastQuery();
    QMapIterator<QString, QVariant> it(q.boundValues());
    while (it.hasNext()) {
        it.next();
        QVariant value = it.value();
        switch (it.value().type()) {
        case QVariant::String:
            value = QString("'%1'").arg(value.toString().replace("'", "''"));
            break;
        case QVariant::Date:
            value = QString("'%1'").arg(value.toDate().toString("dd.MM.yyyy"));
            break;
        case QVariant::DateTime:
            value = QString("'%1'").arg(value.toDateTime().toString("dd.MM.yyyy"));
            break;
        case QVariant::Double:
            value = QString("%1").arg(value.toDouble());
            break;
        case QVariant::Int:
            value = QString("%1").arg(value.toInt());
            break;
        default:
            break;
        }
        sql.replace(it.key(), value.toString());
    }
    qDebug() << sql;

    m_db.close();
    return true;
}

bool GridWindow2::del(const QString &table, const QMap<QString, QVariant> &where)
{
    m_db.open();
    QSqlQuery q(m_db);
    QString sql = "DELETE FROM " + table + " ";

    if (where.count()) {
        sql += "WHERE ";
        for (QMap<QString, QVariant>::const_iterator i = where.begin(); i != where.end(); i++)
            sql += i.key() + "=:" + i.key() + " AND ";
        sql = sql.remove(sql.length() - 4, 4);
    }

    if (!q.prepare(sql)) {
        QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
        m_db.close();
        return false;
    }

    if (where.count())  {
        for (QMap<QString, QVariant>::const_iterator i = where.begin(); i != where.end(); i++)
            q.bindValue(i.key(), i.value());
    }

    if (!q.exec()) {
        QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
        m_db.close();
        return false;
    }

    m_db.close();
    return true;
}

void GridWindow2::setDefaultFilter()
{
}

void GridWindow2::setData(const int &row, const QVector<QVariant> &values)
{
    for (int i = 0; i < values.count(); i++)
        m_pTable->item(row, i)->setData(Qt::DisplayRole, values[i]);
}

void GridWindow2::setDataFromDialog(const int &row, const QDialog *dialog)
{
    Q_UNUSED(dialog);
    Q_UNUSED(row);
}

void GridWindow2::getData(const int &row, QMap<QString, QVariant> &values)
{
    for (int i = 0; i < m_columns.count(); i++)
        values[m_columns[i]] = m_pTable->item(row, i)->data(Qt::DisplayRole);
}

int GridWindow2::getSelectedRows(QList<int> &rows)
{
    QList<QTableWidgetItem *> ti = m_pTable->selectedItems();
    for (QList<QTableWidgetItem *>::const_iterator i = ti.begin(); i != ti.end(); i++) {
        if (!rows.contains((*i)->row()))
            rows.push_back((*i)->row());
    }

    return rows.count();
}

void GridWindow2::slotNew()
{
}

void GridWindow2::slotEdit()
{
}

void GridWindow2::slotDelete()
{
}

void GridWindow2::slotFilter()
{
}

void GridWindow2::slotRefresh()
{
}

void GridWindow2::slotExportToExcel()
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

    for (int i = 0; i < m_pTable->columnCount(); i++)
    {
        sheet_set_cell_value(sheet, i + 1, 1, m_pTable->horizontalHeaderItem(i)->text().toStdWString().c_str());
        sheet_set_col_width(sheet, i + 1, m_pTable->columnWidth(i) / 7);
    }

    for (int i = 0; i < m_pTable->rowCount(); i++)
        for (int j =  0; j < m_pTable->columnCount(); j++)
            sheet_set_cell_value(sheet, j + 1, i + 2, m_pTable->item(i, j)->text().toStdWString().c_str());

    excel_show(excel);
    clear(sheet);
    clear(workbook);
    clear(excel);
}

void GridWindow2::slotAction()
{
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    activateWindow();
    setFocus();
}
