#ifndef QCOSTUMDISHDIALOG_H
#define QCOSTUMDISHDIALOG_H

#include "qbasedialog.h"

class QCostumDishDialog : public QBaseDialog
{
    Q_OBJECT
public:
    explicit QCostumDishDialog(QWidget *parent = 0);
    
private:
    QListWidget *m_pGoods;
    QListWidget *m_pTotal;
    QListWidget *m_pRecipe;


public slots:
    
};

#endif // QCOSTUMDISHDIALOG_H
