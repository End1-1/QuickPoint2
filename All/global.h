#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QVariant>
#include <QMap>
#include <QVector>
#include <QDate>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

#include "../All/classes.h"

#define ROLE_EDIT_PARTIAL_DATA 1
#define ROLE_EDIT_ALL_DATA 2
#define ROLE_EDIT_STORE_DOCS 3
#define ROLE_VIEW_SALE_DOCS 4
#define ROLE_DEL_SALE_DOCS 5
#define ROLE_EDIT_SALARY_1 6 //Only with formula
#define ROLE_EDIT_SALARY_2 7 //Free, can input any value
#define ROLE_MAKE_SALE 8
#define ROLE_RECIPE_ON_FLY 9
#define ROLE_DEL_STORE_DOC 10

#define SETTINGS_COMPORT 0
#define SETTINGS_LOCAL_PRINTER 1
#define SETTINGS_CHECK_HEADER 2
#define SETTINGS_AUTO_DATE_CHANGE_TIME 3
#define SETTINGS_COSTUMER_DISPLAY_MESSAGE 4
#define SETTINGS_DEFAULT_MENU 5
#define SETTINGS_FONT_NAME 6
#define SETTINGS_FONT_SIZE 7
#define SETTINGS_FONT_BOLD 8
#define SETTINGS_ID_PREFIX 9
#define SETTINGS_TABLE_ID 10
#define SETTINGS_PRINTER_ALIAS 11
#define SETTINGS_PRINTER_PAYMENT 12
#define SETTINGS_PRINT_LOGO_SIZE 13
#define SETTINGS_PRINTER_PAPER_WIDTH 14
#define SETTINGS_PRINT_PAYMENT_HEADER 15
#define SETTINGS_PRINT_PAYMENT_BOTTOM 16
#define SETTINGS_DATETIME_FORMAT 17
#define SETTINGS_DATE_FORMAT 18
#define SETTINGS_TIME_FORMAT 19
#define SETTINGS_COMPORT_STRING_LENGTH 20

#define ORDER_ID 0
#define ORDER_STATUS_ID 1
#define ORDER_DATE_CASH 2
#define ORDER_TABLE_ID 3
#define ORDER_WAITER_ID 4
#define ORDER_AMOUNT 5

#define ORDER_STATUS_OPENED 1
#define ORDER_STATUS_CLOSED 2

#define MOD_NOTE 1
#define MOD_PAYMENT_DATETIME 2

namespace Data {
    extern QString homeDir;
    extern QString connectionsDir;

    extern int databaseIndex;

    extern QString computerName;

    extern QMap<int, QVariant> settings;

    void InitSettings();
    QDate cashDate();
}

#endif // GLOBAL_H
