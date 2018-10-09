#ifndef CHANGE_H
#define CHANGE_H

#include <QDialog>
#include <QtGui>
#include <QLineEdit>

class change : public QDialog
{
    Q_OBJECT
public:
    explicit change(double bill, QWidget *parent = 0);

private:
    QLineEdit *m_bill;
    QLineEdit *m_change;
    void btnSize(QPushButton *btn);

private slots:
    void slotAmount();
};

#endif // CHANGE_H
