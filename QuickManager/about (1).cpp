#include "about.h"
#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

About::About(QWidget *parent) :
    QDialog(parent)
{
    QString module = "Manager";

#ifdef QPOINT
    module = "Sales point";
#endif

    QLabel *about = new QLabel("<img src=':icons/app.ico' width=50 height=50></img><br><b>QuickPoint - " + module
                               + "<br>Version 2.3.42</b><br><br>(c)2013-2018 Kudryashov Vasily<br>Email: end1_1@yahoo.com<br><br>", this);

    QPushButton *pbOk = new QPushButton(tr("Close"), this);
    connect(pbOk, SIGNAL(clicked()), SLOT(accept()));
    QVBoxLayout *l = new QVBoxLayout(this);

    l->addWidget(about);
    l->addStretch(1);
    l->addWidget(pbOk, Qt::AlignCenter);
    setLayout(l);
}
