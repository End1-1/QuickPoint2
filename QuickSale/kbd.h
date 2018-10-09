#ifndef KBD_H
#define KBD_H

#include <QWidget>

const QString keys [4][3] = {
    {"7", "8", "9"},
    {"4", "5", "6"},
    {"1", "2", "3"},
    {"0", "C", "."}
};

class Kbd : public QWidget
{
    Q_OBJECT
public:
    explicit Kbd(QWidget *parent = 0);
    
signals:
    void signalChar(const QString &c);
    
private slots:
    void slotClick();
    
};

#endif // KBD_H
