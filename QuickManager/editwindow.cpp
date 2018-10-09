#include "editwindow.h"
#include <QtGui>
#include "tablemodel.h"
#include "global.h"
#include "../All/econnection.h"

EditWindow::EditWindow(QWidget *parent) :
    QDialog(parent)
{
    QPushButton *pbtn_Ok = new QPushButton(tr("Ok"));
    QPushButton *pbtn_Cancel = new QPushButton(tr("Cancel"));
    connect(pbtn_Ok, SIGNAL(clicked()), SLOT(slotOk()));
    connect(pbtn_Cancel, SIGNAL(clicked()), SLOT(slotCancel()));
    QHBoxLayout *l = new QHBoxLayout();
    l->addStretch(1);
    l->addWidget(pbtn_Ok);
    l->addWidget(pbtn_Cancel);
    l->addStretch(1);
    m_pwidget = new QWidget();
    m_pwidget->setLayout(l);

    EConnection::setConnectionParams(m_db.db, Data::databaseIndex);
}

void EditWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Escape:
    case Qt::Key_Return:
        return;
    default:
        QDialog::keyPressEvent(e);
    }
}

QPushButton *EditWindow::newBtn(QWidget *parent)
{
    QPushButton *btn = new QPushButton("...", parent);
    btn->setMaximumSize(25, 25);
    return btn;
}

void EditWindow::slotOk()
{
    if (checkData())
        accept();
}

void EditWindow::slotCancel()
{
    reject();
}
