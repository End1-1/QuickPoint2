#ifndef QPASSWORDDIALOG_H
#define QPASSWORDDIALOG_H

#include "qbasedialog.h"

class QPasswordDialog : public QBaseDialog
{
    Q_OBJECT
public:
    explicit QPasswordDialog(QWidget *parent = 0);
    QString m_password;

signals:
    void Finish(QString value);
    
public slots:
    void OnFinish(QString value);
    
};

#endif // QPASSWORDDIALOG_H
