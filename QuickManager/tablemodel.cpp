#include "tablemodel.h"
#include <QtSql>
#include <QMessageBox>

TableModel::TableModel(QObject *parent) :
    QAbstractTableModel(parent), l(QLocale::c())
{
    m_rowCount = 0;
    m_colCount = 0;
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_rowCount;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_colCount;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    beginResetModel();
    if (!index.isValid())
        return false;

    if (role == Qt::EditRole)
        m_data[dataIndex(index.row(), index.column())] = value;
    endResetModel();

    return true;
}

bool TableModel::setData(const int row, const int col, const QVariant &value)
{
    return setData(index(row, col), value, Qt::EditRole);
    //emit dataChanged(index(row, col), index(row, col));
}

bool TableModel::setData(const int row, const QString col, const QVariant &value)
{
    return setData(row, m_colName[col], value);
}

int TableModel::dataIndex(int row, int col) const
{
    return (row * m_colCount) + col;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        QVariant d = const_cast<TableModel *>(this)->m_data.at(dataIndex(index.row(), index.column()));
        switch(m_columns[index.column()].DataType)
        {
        case DATA_DATE:
            return l.toString(d.toDate(), "dd.MM.yyyy");
            break;
        case DATA_DATETIME:
            return l.toString(d.toDateTime(), "dd.MM.yyyy HH:mm:ss");
            break;
        case 496: //Integer
        case 501: //Smallint
            return l.toString(d.toInt());
            break;
        case 483: //Float
        case DATA_DOUBLE:
            return l.toString(d.toDouble(), 'f', m_columns[index.column()].RoundTo);
            break;
        default: //449 string
            return d.toString();
        }
    }

    return QVariant();
}

QVariant TableModel::data(const int col, const int row) const
{
    return data(index(row, col), Qt::DisplayRole);
}

QVariant TableModel::data(const QString col, const int row) const
{
    return data(m_colName[col], row);
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            if (section > m_columns.count() - 1) {
                return "";
            }
            return m_columns[section].Caption;
        }
        else
            return ++section;
    }
    return QVariant();
}

double TableModel::Double(const QString &col, const int &row) const
{
    return const_cast<TableModel *>(this)->m_data.at(dataIndex(row, m_colName[col])).toDouble();
}

float TableModel::Float(const QString &col, const int &row) const
{
    return const_cast<TableModel *>(this)->m_data.at(dataIndex(row, m_colName[col])).toFloat();
}

int TableModel::Integer(const QString &col, const int &row) const
{
    return const_cast<TableModel *>(this)->m_data.at(dataIndex(row, m_colName[col])).toInt();
}

int TableModel::Integer(const int &col, const int &row) const
{
    return const_cast<TableModel *>(this)->m_data.at(dataIndex(row, col)).toInt();
}

QString TableModel::String(const QString &col, const int &row) const
{
    return const_cast<TableModel *>(this)->m_data.at(dataIndex(row, m_colName[col])).toString();
}

QString TableModel::String(const int &col, const int &row) const
{
    return const_cast<TableModel *>(this)->m_data.at(dataIndex(row, col)).toString();
}

bool TableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    m_colCount = count;

    for (int i = 0; i < count; i++)
    {
        m_colName[QString::number(i)] = i;
        m_columns.push_back(Column());
    }

    endInsertColumns();
    return true;
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if (count < 1)
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count * m_colCount; i++)
        m_data.push_back(QVariant());
    m_rowCount += count;
    endInsertRows();
    return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if (row > m_rowCount - 1)
        return false;
    if (row + count > m_rowCount - 1)
        count = m_rowCount - row;

    beginRemoveRows(parent, row, row + count - 1);
    if (row < m_rowCount - 1)
        for (int i = row; i < m_rowCount - count; i++)
            rowCopy(i, i + count);
    m_rowCount -= count;
    endRemoveRows();

    emit dataChanged(index(row ,0), index(m_rowCount - 1, m_colCount - 1));
    return true;
}

bool TableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    if (count < 1)
        return false;
    beginRemoveColumns(parent, column, column + count);
    endRemoveColumns();
    return true;
}

void TableModel::rowCopy(const int dest, const int src)
{
    int b1 = dataIndex(dest, 0), b2 = dataIndex(dest, m_colCount), offcet = dataIndex(src, 0);
    if (b1 > b2)
        return;
    for (int i = b1; i != b2; i++)
        m_data[i] = m_data.at(i % m_colCount + offcet);
}

void TableModel::rowReplace(const int &first, const int &last)
{
    insertRows(rowCount(QModelIndex()) - 1, 1, QModelIndex());
    rowCopy(rowCount(QModelIndex()) - 1, first);
    rowCopy(first, last);
    rowCopy(last, rowCount(QModelIndex()) - 1);
    removeRows(rowCount(QModelIndex()) - 1, 1, QModelIndex());
}

void TableModel::clearData()
{
    m_colCount = 0;
    m_rowCount = 0;
    m_data.clear();
    m_colName.clear();
    m_columns.clear();
    resetInternalData();
}

void TableView::ExecuteQuery(QString sql)
{
    m_model.clearData();
    if (!m_db.db.open())
    {
        QMessageBox::critical(this, "SQL error", m_db.db.lastError().databaseText());
        return;
    }
    QSqlQuery query(m_db.db);
    if (!query.exec(sql))
    {
        QMessageBox::critical(this, "SQL error", query.lastError().databaseText());
        return;
    }

    QSqlRecord rec = query.record();
    m_model.insertColumns(0, rec.count(), QModelIndex());
    m_model.m_colName.clear();
    for (int i = 0; i < rec.count(); i++)
    {
        m_model.m_colName[rec.fieldName(i)] = i;
        m_model.m_columns[i].Caption = rec.fieldName(i);
        m_model.m_columns[i].FieldName = rec.fieldName(i);
        m_model.m_columns[i].Width = 100;
        m_model.m_columns[i].DataType = rec.field(i).typeID();
        m_model.m_columns[i].RoundTo = 2;
    }

    int row = 0, cols = m_model.columnCount(QModelIndex());
    while (query.next())
    {
        row = addRow();
        for (int i = 0; i < cols; i++)
            m_model.setData(row, i, query.value(i));
    }
    m_db.db.close();
}

QSqlQuery *TableView::openDB()
{
    if (!m_db.db.open())
    {
        QMessageBox::critical(0, tr("Database error"), m_db.db.lastError().databaseText());
        return 0;
    }
    return new QSqlQuery(m_db.db);
}

void TableView::closeDB(QSqlQuery *query)
{
    delete query;
    m_db.db.close();
}

int TableView::addRow()
{
    m_model.insertRows(m_model.rowCount(QModelIndex()), 1, QModelIndex());
    return m_model.rowCount(QModelIndex()) - 1;
}


TableView::TableView(QObject *parent)
{
    Q_UNUSED(parent);
    setModel(&m_model);
}

TableView::~TableView()
{
}

void TableView::setColumnCaption(const QString &fieldName, const QString &caption)
{
    m_model.m_columns[m_model.m_colName[fieldName]].Caption = caption;
}

void TableView::setColumnWidth(const QString &fieldName, const int &width)
{
    QTableView::setColumnWidth(m_model.m_colName[fieldName], width);
}

void TableView::setColumnDataType(const QString &fieldName, const int type)
{
    m_model.m_columns[m_model.m_colName[fieldName]].DataType = type;
}

void TableView::setColumnRoundTo(const QString &fieldName, const int &roundTo)

{
    m_model.m_columns[m_model.m_colName[fieldName]].RoundTo = roundTo;
}

void TableView::setColumn(const QString &fieldName, const QString &caption, const int &width, const int &type, const int &roundTo)
{
    if (!m_model.m_colName.contains(fieldName))
        return;
    m_model.m_columns[m_model.m_colName[fieldName]].FieldName = fieldName;
    setColumnCaption(fieldName, caption);
    setColumnWidth(fieldName, width);
    setColumnDataType(fieldName, type);
    setColumnRoundTo(fieldName, roundTo);
}

void TableView::setColumn(const int index, const QString &fieldName, const QString &caption, const int &width, const int &type, const int &roundTo)
{
    m_model.m_colName[fieldName] = index;
    setColumn(fieldName, caption, width, type, roundTo);
}

void TableView::getSelectedRows(QVector<int> &rows)
{
    QModelIndexList list = selectionModel()->selectedRows();
    for (QModelIndexList::const_iterator i = list.begin(); i != list.end(); i++)
        if (!rows.contains(i->row()))
            rows.push_back(i->row());
}

bool TableView::checkRowsCount(QVector<int> &rows, const bool &multiselection)
{
    getSelectedRows(rows);
    bool result = true;
    QString msg;

    if (rows.count() == 0)
    {
        msg += tr("Nothing is selected\n");
        result = false;
    }

    if (!multiselection)
        if (rows.count() > 1)
        {
            msg += tr("Multiple selection\n");
            result = false;
        }

    if (!result)
        QMessageBox::critical(this, tr("Error"), msg);

    return result;
}
