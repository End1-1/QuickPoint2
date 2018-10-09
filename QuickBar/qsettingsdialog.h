#ifndef QSETTINGSDIALOG_H
#define QSETTINGSDIALOG_H

#include "qbasedialog.h"

class QSettingsDialog : public QBaseDialog
{
    Q_OBJECT

public:
    explicit QSettingsDialog(QWidget *parent = 0);
    static Connection connection;
    static void readConnection();
    bool checkPassword(const QString &password);

private:
    QLineEdit *m_pConnectionString;
    QLineEdit *m_pUsername;
    QLineEdit *m_pPassword;
    QLineEdit *m_pSettingsPassword;
    
private slots:
    virtual void onOk();
};

#endif // QSETTINGSDIALOG_H
