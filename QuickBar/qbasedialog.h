#ifndef QBASEDIALOG_H
#define QBASEDIALOG_H

#include <QtGui>
#include <QDialog>
#include "global.h"

class QBaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QBaseDialog(QString title, QWidget *parent = 0);

protected:
    QEDatabase m_db;
    QPushButton *qpushbutton(const QString &text);
    QWidget *m_pwdtOkCancel();
    virtual bool isOk();
    
protected slots:
    virtual void onOk();
    virtual void onCancel();    
};

class QEPushButton : public QPushButton
{
public:
    enum ButtonState {TableOpened = 0};

    QEPushButton(const int index, const QString &text, QWidget *parent);
    int Index;
    void setState(const ButtonState state);
};

#endif // QBASEDIALOG_H
