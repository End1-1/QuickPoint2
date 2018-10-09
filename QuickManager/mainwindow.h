#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include "gridwindow.h"
#include "gridwindow2.h"

class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QToolBar *m_pToolBar;
    QMdiArea *m;

private:
   QMenu *m_pmenuFile;
   QMenu *m_pmenuDict;
   QMenu *m_pmenuDocs;
   QMenu *m_pmenuHelp;
   QAction *m_pChangePassword;

   template<class T>
   void addWindow()
   {
       T *t = new T();
       m->addSubWindow(t);
       dynamic_cast<GridWindow *>(t)->setOnToolBar(m_pToolBar);
       dynamic_cast<GridWindow *>(t)->show();
   }

   template<class T>
   void addWindow2()
   {
       T *t = new T();
       m->addSubWindow(t);
       dynamic_cast<GridWindow2 *>(t)->show();
       dynamic_cast<GridWindow2 *>(t)->setOnToolBar(m_pToolBar);
   }

   void buildMenu();
   void destroyMenu();

private slots:
   void slotConnect();
   void slotDisconnect();
   void slotExit();
   void slotPositions();
   void slotEmployees();
   void slotRoles();
   void slotUnits();
   void slotGroupFood();
   void slotFood();
   void slotPrinter();
   void slotMenu();
   void slotGroupDish();
   void slotModifiers();
   void slotDish();
   void slotStore();
   void slotDocumentsInStore();
   void slotStoreBalance();
   void slotSales();
   void slotCardsHistory();
   void slotTotalResult();
   void slotSalaryReport();
   void slotAbout();
   void slotChangePassword();
   void slotHall();
   void slotTables();
   void slotPrintAliases();
   void slotPrintSchema();
   void slotSettings();
   void slotCards();
   void slotCardExtendedStatistics();
};

#endif // MAINWINDOW_H
