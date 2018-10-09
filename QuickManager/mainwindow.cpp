#include "mainwindow.h"
#include <QtGui>
#include "lunits.h"
#include "lmenu.h"
#include "ldishgroup.h"
#include "dishes2.h"
#include "lstorage.h"
#include "lposition.h"
#include "lroles.h"
#include "lemployee.h"
#include "lgoodsgroups.h"
#include "lprinter.h"
#include "rcardbyuse.h"
#include "lgoods.h"
#include "rsales.h"
#include "rstore.h"
#include "rfinalreport.h"
#include "about.h"
#include "lmodifiers.h"
#include "echangepassword.h"
#include "rsalary.h"
#include "lhall.h"
#include "ltables.h"
#include "lprintalieases.h"
#include "lprinterschema.h"
#include "lsettings.h"
#include "rcardappend.h"
#include "global.h"
#include "login.h"
#include "rstorebalance.h"
#include "defines.h"
#include "rcardstatistic.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m = new QMdiArea();
    setCentralWidget(m);

    m_pmenuFile = new QMenu(tr("File"), 0);
    m_pmenuFile->addAction(tr("Connect"), this, SLOT(slotConnect()));
    m_pmenuFile->addAction(tr("Disconnect"), this, SLOT(slotDisconnect()));
    m_pChangePassword = m_pmenuFile->addAction(tr("Change password"), this, SLOT(slotChangePassword()));
    m_pChangePassword->setEnabled(false);
    m_pmenuFile->addAction(tr("Exit"), this, SLOT(slotExit()));

    m_pmenuDict = new QMenu(tr("Dictionaries"), 0);
    m_pmenuDict->setEnabled(false);
    m_pmenuDocs = new QMenu(tr("Documents"), 0);
    m_pmenuDocs->setEnabled(false);

    m_pmenuHelp = new QMenu(tr("Help"), 0);
    m_pmenuHelp->addAction(tr("About"), this, SLOT(slotAbout()));

    menuBar()->addMenu(m_pmenuFile);
    menuBar()->addMenu(m_pmenuDict);
    menuBar()->addMenu(m_pmenuDocs);
    menuBar()->addMenu(m_pmenuHelp);

    addToolBar(Qt::BottomToolBarArea, m_pToolBar = new QToolBar(this));

    fMainWindow = this;
}

void MainWindow::buildMenu()
{
    if (M::user.m_roles.contains(ROLE_EDIT_ALL_DATA))
    {
        m_pmenuDict->addAction(tr("Positions"), this, SLOT(slotPositions()));
        m_pmenuDict->addAction(tr("Employees"), this, SLOT(slotEmployees()));
        m_pmenuDict->addAction(tr("Roles"), this, SLOT(slotRoles()));
        m_pmenuDict->addSeparator();
    }

    if (M::user.m_roles.contains(ROLE_EDIT_PARTIAL_DATA) || M::user.m_roles.contains(ROLE_EDIT_ALL_DATA))
    {
        m_pmenuDict->addAction(tr("Units"), this, SLOT(slotUnits()));
        m_pmenuDict->addAction(tr("Group food"), this, SLOT(slotGroupFood()));
        m_pmenuDict->addAction(tr("Food"), this, SLOT(slotFood()));
        m_pmenuDict->addSeparator();
        m_pmenuDict->addAction(tr("Menu"), this, SLOT(slotMenu()));
        m_pmenuDict->addAction(tr("Group dish"), this, SLOT(slotGroupDish()));
        m_pmenuDict->addAction(tr("Dish"), this, SLOT(slotDish()));
        m_pmenuDict->addAction(tr("Modifiers"), this, SLOT(slotModifiers()));
        m_pmenuDict->addSeparator();
        m_pmenuDict->addAction(tr("Printers"), this, SLOT(slotPrinter()));
        m_pmenuDict->addAction(tr("Printers aliases"), this, SLOT(slotPrintAliases()));
        m_pmenuDict->addAction(tr("Printers schemas"), this, SLOT(slotPrintSchema()));
        m_pmenuDict->addSeparator();
        m_pmenuDict->addAction(tr("Hall"), this, SLOT(slotHall()));
        m_pmenuDict->addAction(tr("Tables"), this, SLOT(slotTables()));
        m_pmenuDict->addSeparator();
        m_pmenuDict->addAction(tr("Storages"), this, SLOT(slotStore()));
        m_pmenuDict->addSeparator();
        m_pmenuDict->addAction(tr("Settings"), this, SLOT(slotSettings()));
        m_pmenuDict->addAction(tr("Cards"), this, SLOT(slotCards()));
    }

    if (M::user.m_roles.contains(ROLE_EDIT_STORE_DOCS))
    {
        m_pmenuDocs->addAction(tr("Store documents"), this, SLOT(slotDocumentsInStore()));
        m_pmenuDocs->addAction(tr("Total report"), this, SLOT(slotTotalResult()));
        m_pmenuDocs->addAction(tr("Store balance"), this, SLOT(slotStoreBalance()));
    }

    if (M::user.m_roles.contains(ROLE_VIEW_SALE_DOCS)) {
        m_pmenuDocs->addAction(tr("Sales"), this, SLOT(slotSales()));
        m_pmenuDocs->addAction(tr("Cards history"), this, SLOT(slotCardsHistory()));
        m_pmenuDocs->addAction(tr("Cards extended statistics"), this, SLOT(slotCardExtendedStatistics()));
    }

    if (M::user.m_roles.contains(ROLE_EDIT_SALARY_1) || M::user.m_roles.contains(ROLE_EDIT_SALARY_2))
        m_pmenuDocs->addAction(tr("Salary"), this, SLOT(slotSalaryReport()));

    m_pmenuDict->setEnabled(true);
    m_pmenuDocs->setEnabled(true);
}

void MainWindow::destroyMenu()
{
    m_pmenuDict->clear();
    m_pmenuDocs->clear();
    m_pmenuDict->setEnabled(false);
    m_pmenuDocs->setEnabled(false);
    m_pChangePassword->setEnabled(false);
}

void MainWindow::slotConnect()
{
    slotDisconnect();
    Login *l = new Login(this);
    if (l->exec() == QDialog::Accepted)
    {
        buildMenu();
        m_pChangePassword->setEnabled(true);
    }
    delete l;
}

void MainWindow::slotDisconnect()
{
    m->closeAllSubWindows();
    destroyMenu();
    M::user.m_roles.clear();
}

void MainWindow::slotExit()
{
    close();
}

void MainWindow::slotPositions()
{
    addWindow<LPosition>();
}

void MainWindow::slotEmployees()
{
    addWindow<LEmployee>();
}

void MainWindow::slotRoles()
{
    addWindow<LRoles>();
}

void MainWindow::slotUnits()
{
    addWindow<LUnits>();
}

void MainWindow::slotGroupFood()
{
    addWindow<LGoodsGroups>();
}

void MainWindow::slotFood()
{
    addWindow<LGoods>();
}

void MainWindow::slotPrinter()
{
    addWindow<LPrinter>();
}

void MainWindow::slotMenu()
{
    addWindow<LMenu>();
}

void MainWindow::slotGroupDish()
{
    addWindow<LDishGroup>();
}

void MainWindow::slotModifiers()
{
    addWindow<LModifiers>();
}

void MainWindow::slotDish()
{
    addWindow2<Dishes2>();
}

void MainWindow::slotStore()
{
    addWindow<LStorage>();
}

void MainWindow::slotDocumentsInStore()
{
    addWindow<RStore>();
}

void MainWindow::slotStoreBalance()
{
    addWindow<RStoreBalance>();
}

void MainWindow::slotSales()
{
    addWindow<RSales>();
}

void MainWindow::slotCardsHistory()
{
    addWindow<rcardbyuse>();
}


void MainWindow::slotTotalResult()
{
    addWindow<RFinalReport>();
}

void MainWindow::slotSalaryReport()
{
    addWindow<RSalary>();
}

void MainWindow::slotAbout()
{
    About *a = new About(this);
    a->exec();
    delete a;
}

void MainWindow::slotChangePassword()
{
    EChangePassword *e = new EChangePassword(this);
    e->exec();
    delete e;
}

void MainWindow::slotHall()
{
    addWindow<LHall>();
}

void MainWindow::slotTables()
{
    addWindow<LTables>();
}

void MainWindow::slotPrintAliases()
{
    addWindow<LPrintAlieases>();
}

void MainWindow::slotPrintSchema()
{
    addWindow<LPrinterSchema>();
}

void MainWindow::slotSettings()
{
    addWindow<LSettings>();
}

void MainWindow::slotCards()
{
    addWindow<rcardappend>();
}

void MainWindow::slotCardExtendedStatistics()
{
    addWindow<RCardStatistic>();
}
