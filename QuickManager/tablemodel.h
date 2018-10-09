#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QtSql>
#include <QTableView>
#include <QSqlQuery>
#include <QList>
#include "../All/db.h"

#define DATABASE_DRIVER "QIBASE"

#define DATA_STRING 0
#define DATA_DATE 1
#define DATA_DATETIME 2
#define DATA_INTEGER 496
#define DATA_DOUBLE 497

typedef struct {
    QString Caption;
    QString FieldName;
    int Width;
    int DataType;
    int RoundTo; //Scale of double, float, etc...
} Column;

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    int m_rowCount;
    int m_colCount;
    int dataIndex(int row, int col) const;
    const QLocale l;

public:
    QList<QVariant> m_data;
    QList<Column> m_columns;
    QMap<QString, int> m_colName;

    explicit TableModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setData(const int row, const int col, const QVariant &value);
    bool setData(const int row, const QString col, const QVariant &value);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant data(const int col, const int row) const;
    QVariant data(const QString col, const int row) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    double Double(const QString &col, const int &row) const;
    float Float(const QString &col, const int &row) const;
    int Integer(const QString &col, const int &row) const;
    int Integer(const int &col, const int &row) const;
    QString String(const QString &col, const int &row) const;
    QString String(const int &col, const int &row) const;
    bool insertColumns(int column, int count, const QModelIndex &parent);
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool removeColumns(int column, int count, const QModelIndex &parent);
    void rowCopy(const int dest, const int src);
    void rowReplace(const int &first, const int &last);
    void clearData();
};

class TableView : public QTableView
{
   Q_OBJECT

public:
    Database m_db;

public:
    explicit TableView(QObject *parent = 0);
    ~TableView();
    TableModel m_model;
    void ExecuteQuery(QString sql);
    QSqlQuery *openDB();
    void closeDB(QSqlQuery *query);
    int addRow();
    void setColumnCaption(const QString &fieldName, const QString &caption);
    void setColumnWidth(const QString &fieldName, const int &width);
    void setColumnDataType(const QString &fieldName, const int type);
    void setColumnRoundTo(const QString &fieldName, const int &roundTo);
    void setColumn(const QString &fieldName, const QString &caption, const int &width, const int &type, const int &roundTo = 0);
    void setColumn(const int index, const QString &fieldName, const QString &caption, const int &width, const int &type, const int &roundTo = 0);
    void getSelectedRows(QVector<int> &rows);
    bool checkRowsCount(QVector<int> &rows, const bool &multiselection = true);
};

#endif // TABLEMODEL_H
