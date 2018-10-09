#ifndef QFACEWINDOW_H
#define QFACEWINDOW_H

#include <QtGui>
#include <QDialog>

class QNumpadWidget;

class QFaceWindow : public QDialog
{
    Q_OBJECT
public:
    explicit QFaceWindow(QWidget *parent = 0);

private:
    QNumpadWidget *m_pwdtNumpad;
    void setDefault();
    
signals:
    
public slots:
    void onSettings();
    void onEnter(const QString &password);
};

#endif // QFACEWINDOW_H
