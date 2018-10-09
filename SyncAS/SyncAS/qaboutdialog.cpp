#include "qaboutdialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

QAboutDialog::QAboutDialog(QWidget *parent) :
    QDialog(parent)
{
    QLabel *plblAbout = new QLabel("<html><body>ASAccountant synchronization v.1.1"
                              "<br>(C)2013 Vasily Kudryashov<br>"
                              "E-mail: end1_1@yahoo.com</body></html>", this);
    QPushButton *pbtnOk = new QPushButton("Close", this);
    QVBoxLayout *pvlMain = new QVBoxLayout(this);

    connect(pbtnOk, SIGNAL(clicked()), SLOT(accept()));

    pvlMain->addWidget(plblAbout);
    pvlMain->addWidget(pbtnOk);
    setLayout(pvlMain);
}
