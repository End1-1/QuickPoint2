#ifndef GRIDWINDOW2_H
#define GRIDWINDOW2_H

#include <QMainWindow>
#include <QToolBar>
#include <QtSql>
#include <QTableWidget>

typedef int* (*Excel_Create)();
typedef int (*Excel_Show)(void*);
typedef int* (*Workbook_Add)(void *);
typedef int* (*Sheet_Select)(void *, int);
typedef int (*Sheet_Set_Cell_Value)(void*, int, int, const wchar_t *);
typedef int (*Sheet_Set_Col_Width)(void *, int, int);
typedef int (*Clear)(void*);

class GridWindow2 : public QMainWindow
{
    Q_OBJECT

private:
    QString m_currentDbNumber;
    static int m_dbNumber;
    QAction *m_pAction;

public:
    explicit GridWindow2(QString title, QWidget *parent = 0);
    ~GridWindow2();
    void setOnToolBar(QToolBar *toolBar);
    
protected:
    QToolBar *m_pToolBar;
    QTableWidget *m_pTable;
    QSqlDatabase m_db;
    QSqlQuery *m_pQuery;
    QStringList m_sqlWhere;
    QStringList m_columns;
    QStringList m_headerCaptions;
    QVector<int> m_headerWidths;

    void initDatabase(QString dbPath, QString username, QString password);
    bool query(QSqlQuery &q, const QString &tables, const QStringList &columns, const QStringList &where, QStringList &order);
    int insert(const QString &genId, const QString &table, QMap<QString, QVariant> &values);
    bool update(const QString &table, const QMap<QString, QVariant> &values, const QMap<QString, QVariant> &where);
    bool del(const QString &table, const QMap<QString, QVariant> &where);
    virtual void setDefaultFilter();
    void setData(const int &row, const QVector<QVariant> &values);
    virtual void setDataFromDialog(const int &row, const QDialog *dialog);
    void getData(const int &row, QMap<QString, QVariant> &values);

    int getSelectedRows(QList<int> &rows);

protected slots:
    virtual void slotNew();
    virtual void slotEdit();
    virtual void slotDelete();
    virtual void slotFilter();
    virtual void slotRefresh();
    virtual void slotExportToExcel();
    void slotAction();
};

#endif // GRIDWINDOW2_H
