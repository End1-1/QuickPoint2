#ifndef QMENUWINDOW_H
#define QMENUWINDOW_H

#include "qbasedialog.h"

class QMenuWindow : public QBaseDialog
{
    Q_OBJECT
public:
    explicit QMenuWindow(QWidget *parent = 0);
    int m_actionCode;

private:
    QListWidget *m_pActions;

private slots:
    void OnAction(QModelIndex index);
    
};

#endif // QMENUWINDOW_H
