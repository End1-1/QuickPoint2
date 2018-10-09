#ifndef QLISTDIALOG_H
#define QLISTDIALOG_H

#include "qbasedialog.h"

class QListDialog : public QBaseDialog
{
    Q_OBJECT
public:
    explicit QListDialog(QWidget *parent = 0);
    QListWidget *m_pList;
    int m_selectedIndex;
    
public slots:
    void OnDown();
    void OnUp();
    void OnListClicked(QModelIndex index);
    
};

#endif // QLISTDIALOG_H
