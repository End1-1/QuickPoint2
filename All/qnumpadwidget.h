#ifndef QNUMPADWIDGET_H
#define QNUMPADWIDGET_H

#include <QtGui>
#include "global.h"


#define MINSIZE QSize(50, 50)

class QNumpadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QNumpadWidget(QWidget *parent = 0);
    QLineEdit *m_pInput;
    void setDefault();    
    QPushButton *qpushbutton(const QString &text);

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void Finish(QString value);
    void onNumKey();
    
public slots:
    void onNumKeyPress();
    void onOkKeyPress();
    void onClear();
    
};

#endif // QNUMPADWIDGET_H
