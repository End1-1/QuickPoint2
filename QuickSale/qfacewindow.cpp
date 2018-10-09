#include "qfacewindow.h"
#include "qmessagedialog.h"
#include "../All/qnumpadwidget.h"
#include "../All/econnection.h"
#include "data.h"
#include "../All/classes.h"
#include "mainwidget.h"
#include "../QuickManager/about.h"

QFaceWindow::QFaceWindow(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint)
{
    QGridLayout *plMain = new QGridLayout();
    m_pwdtNumpad = new QNumpadWidget(this);
    QPushButton *pbtn_Settings = new QPushButton(tr("Connection"), this),
                *pbtn_Quit = new QPushButton(tr("Exit"), this);
    pbtn_Settings->setMinimumHeight(50);
    pbtn_Quit->setMinimumHeight(50);

    plMain->setRowStretch(0, 1);
    plMain->addWidget(m_pwdtNumpad, 1, 1);
    plMain->setRowStretch(2, 1);
    plMain->addWidget(pbtn_Settings, 3, 1);
    plMain->addWidget(pbtn_Quit, 4, 1);
    plMain->setRowStretch(5, 1);
    plMain->setColumnStretch(0, 1);
    plMain->setColumnStretch(2, 1);
    setLayout(plMain);

    connect(m_pwdtNumpad, SIGNAL(Finish(QString)), SLOT(onEnter(QString)));
    connect(pbtn_Settings, SIGNAL(clicked()), SLOT(onSettings()));
    connect(pbtn_Quit, SIGNAL(clicked()), SLOT(reject()));
}

void QFaceWindow::onSettings()
{
    QString password = m_pwdtNumpad->m_pInput->text();
    m_pwdtNumpad->m_pInput->clear();

    if (!EConnection::CheckConnectionPassword(Data::homeDir + "/sale.con", password))
    {
        QMessageDialog::ShowMessage(tr("Access denied"), this);
        return;
    }

    EConnection *e = new EConnection(this);
    e->setConnection(CD);
    if (e->exec() == QDialog::Accepted)
    {
        e->getConnection(CD);
        EConnection::WriteConnection(Data::homeDir + "/sale.con", CD);
        EConnection::setConnectionParams(DB.db, CD);
    }
    delete e;
}

void QFaceWindow::onEnter(const QString &password)
{
    if (password.toUpper() == "ABOUT")
    {
        About *a = new About(this);
        a->exec();
        delete a;
        return;
    }

    CUser u(DB);
    if (!u.checkPassword(password))
    {
        QMessageDialog::ShowMessage(tr("Access denied"), this);
        return;
    }

    MainWidget *m = new MainWidget(this);
    m->setWindowState(Qt::WindowMaximized);
    m->u = u;
    m->showFullScreen();
    m->buildMenu();
    m->exec();
    delete m;
}

