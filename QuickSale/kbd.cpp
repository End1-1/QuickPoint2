#include "kbd.h"
#include <QPushButton>
#include <QGridLayout>

Kbd::Kbd(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(190, 250);
    QGridLayout *l = new QGridLayout(this);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 3; j++)
        {
            QPushButton *pb = new QPushButton(keys[i][j], this);
            pb->setMaximumSize(60, 60);
            connect(pb, SIGNAL(clicked()), SLOT(slotClick()));
            l->addWidget(pb, i, j);
        }
    setLayout(l);
}


void Kbd::slotClick()
{
    QPushButton *pb = qobject_cast<QPushButton *>(sender());
    emit signalChar(pb->text());
}
