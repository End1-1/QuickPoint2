#ifndef ERECIPE_H
#define ERECIPE_H

#include "editwindow.h"
#include <QtGui>
#include "tablemodel.h"

class AStoreFood;

class ERecipe : public EditWindow
{
    Q_OBJECT

public:
    ERecipe(const QString &defaultStoreId, const QString &defaultStoreName, QWidget *parent = 0);
    TableView *table;

private:
    QString m_defaultStoreId;
    QString m_defaultStoreName;
    bool checkData();
    void setData(AStoreFood *a, const int &row);

private slots:
    void slotNew();
    void slotEdit();
    void slotDelete();
    void slotExportToExcel();

};

#endif // ERECIPE_H
