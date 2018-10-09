#ifndef CHECKLIST_H
#define CHECKLIST_H

#include <QDialog>
#include <QtGui>
#include "tablemodel.h"
#include "eunit.h"

class CheckList : public QDialog
{
    Q_OBJECT
public:
    explicit CheckList(bool multiCheck, QWidget *parent = 0);

private:
    bool m_multiCheck;
    QListWidget *plist_Values;

public:
    TableView *table;
    QVector<QVariant> result;
    QVector<int> checked;
    QVector<QVariant> key;
    QVector<QVariant> values;
    void sql(const QString sql, const int column);
    void splitResult(const QString str);
    void makeList();

private slots:
    void slotItemChanged(QModelIndex index);
    void slotOk();

};

#endif // CHECKLIST_H
