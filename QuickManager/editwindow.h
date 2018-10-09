#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QDialog>
#include <QtGui>
#include "tablemodel.h"
#include "data.h"
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QGridLayout>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>
#include <QColorDialog>
#include <QDateEdit>
#include <QTableWidget>
#include <QProgressDialog>

class Database;

class EditWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = 0);
    
protected:
    QWidget *m_pwidget;
    void keyPressEvent(QKeyEvent *e);
    virtual bool checkData() = 0;
    static QPushButton *newBtn(QWidget *parent);
    Database m_db;


protected slots:
    virtual void slotOk();
    virtual void slotCancel();
    
};

#endif // EDITWINDOW_H
