#ifndef DIGITS_H
#define DIGITS_H

#include <QDialog>

class Kbd;
class QLineEdit;

class Digits : public QDialog
{
    Q_OBJECT
public:
    explicit Digits(QWidget *parent = 0);
    static bool value(double &value);

private:
    Kbd *kbd;
    QLineEdit *buffer;

private slots:
    void slotChar(const QString &value);
    
};

#endif // DIGITS_H
