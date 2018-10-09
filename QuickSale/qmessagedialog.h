#ifndef QMESSAGEDIALOG_H
#define QMESSAGEDIALOG_H

#include <QDialog>
#include <QLabel>

class QMessageDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel *m_pMessage;

public:
    explicit QMessageDialog(QWidget *parent = 0);
    static int ShowMessage(const QString &message, QWidget *parent = 0);
    
};

#endif // QMESSAGEDIALOG_H
