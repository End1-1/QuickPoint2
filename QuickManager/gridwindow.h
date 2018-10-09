#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <QMainWindow>
#include "tablemodel.h"
#include "global.h"
#include "data.h"

typedef int* (*Excel_Create)();
typedef int (*Excel_Show)(void*);
typedef int* (*Workbook_Add)(void *);
typedef int* (*Sheet_Select)(void *, int);
typedef int (*Sheet_Set_Cell_Value)(void*, int, int, const wchar_t *);
typedef int (*Sheet_Set_Col_Width)(void *, int, int);
typedef int (*Clear)(void*);

class QSqlQuery;

class GridWindow : public QMainWindow
{
    Q_OBJECT

private:
    QVector<int> selectedRows;
    QAction *m_pAction;

public:
    explicit GridWindow(QWidget *parent = 0);
    ~GridWindow();
    void setOnToolBar(QToolBar *toolBar);

protected:
    TableView *table;
    QToolBar *topToolBar;
    virtual void getData() = 0;
    bool prepare(QSqlQuery *query, const QString &sqlStr);
    bool exec(QSqlQuery *query);
    bool exec(QSqlQuery *query, const QString &sqlStr);
    QVector<int> &getSelectedRows();
    void setColumnCaption(const QString &fieldName, const QString &caption);
    void setColumnWidth(const QString &fieldName, const int &width);
    void setColumnDataType(const QString &fieldName, const int type);
    void setColumn(const QString &fieldName, const QString &caption, const int &width, const int &type);
    const QLocale l;
   // void setColumn(const int index, const QString &fieldName, const QString &caption, const int &width, const int &type);

protected slots:
    virtual void slotNew();
    virtual void slotEdit();
    virtual void slotDelete();
    virtual void slotFilter();
    virtual void slotRefresh();
    virtual void slotTotal();
    virtual void slotExportToExcel();
    virtual void slotPrint();
    void slotAction();
    virtual void doubleClicked(const QModelIndex &index);
signals:
    void dc(const QModelIndex &index);
};

#endif // GRIDWINDOW_H
