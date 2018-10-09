#ifndef QHALLDIALOG_H
#define QHALLDIALOG_H

#include "qbasedialog.h"

#define TABLE_SIZE QSize(150, 60)

class QHallDialog : public QBaseDialog
{
    Q_OBJECT
public:
    explicit QHallDialog(const QUser &user, QWidget *parent = 0);
    ~QHallDialog();
    int m_TableId;
    
private:
    int m_hallFilter;
    const QUser &m_user;
    QWidget *pwdtTable;
    QGridLayout *plTable;
    QScrollArea *m_psaTable;
    QSqlQuery *m_pquery;
    QMap<int, QEPushButton *> m_vecButtons;

    void makeTablesList();
    
public slots:
    void OnBtnScrollDown();
    void OnBtnScrollUp();
    void OnBtnTableClicked();
    void OnBtnHallClicked();
    void OnBtnCashClicked();
    void OnBtnUsersPasswordsClicked();
    
};

#endif // QHALLDIALOG_H
