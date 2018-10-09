#ifndef QMOVEORDERDIALOG_H
#define QMOVEORDERDIALOG_H

#include "qbasedialog.h"

class QMoveOrderDialog : public QBaseDialog
{
    Q_OBJECT
public:
    explicit QMoveOrderDialog(QWidget *parent = 0);
    QListWidget *m_pSrcTable;
    QListWidget *m_pDstTable;
    QLabel *m_pSrcTableName;
    QLabel *m_pDstTableName;
    
private:
    void moveItem(QListWidget *src, QListWidget *dst);
    
public slots:
    void OnMoveLeft();
    void OnMoveRight();
    void OnMoveAllLeft();
    void OnMoveAllRight();
    
};

#endif // QMOVEORDERDIALOG_H
