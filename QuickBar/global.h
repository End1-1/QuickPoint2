#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QMap>
#include <QVariant>
#include <QSqlQuery>
#include <QVector>
#include <QtSql>

#define MINSIZE QSize(50, 50)
#define COLOR_EVEN qRgb(255, 240, 240)
#define COLOR_OGG qRgb(240, 255, 240)
#define DBTYPE "QIBASE"
#define DATE_FORMAT "dd/MM/yyyy"
#define DATETIME_FORMAT "dd/MM/yyyy HH:mm:ss"
#define TIME_FORMAT "HH:mm:ss"

#define S_FONT_NAME 1
#define S_FONT_SIZE 2
#define S_DEFAULT_MENU 3
#define S_ORDER_ID_PREFIX 4
#define S_SERVICE_MARGIN 5
#define S_PRINTER_ALIAS 6
#define S_LOGO_ON_SERVICE_CHECK 7
#define S_CASH_PRINTER 8
#define S_PAYMENT_CHECK_LOGO_SIZE 9
#define S_AUTO_CHANGE_DATE_OF_CASH 10
#define S_DEFAULT_HALL 11

#define ORDER_OPEN 1
#define ORDER_CLOSED 2
#define ORDER_INVALIDATED 3
#define ORDER_EMPTY 4

#define ORDER_MOD_SERVICE_VALUE 1
#define ORDER_MOD_SERVICE_AMOUNT 2
#define ORDER_MOD_COUNTED 3
#define ORDER_MOD_DISCOUNT_VALUE 4
#define ORDER_MOD_DISCOUNT_AMOUNT 5
#define ORDER_MOD_PRINTED_PAYMENT_CHECKS 6
#define ORDER_MOD_TIME_OPEN 7
#define ORDER_MOD_TIME_CLOSE 8

#define ACTION_NONE 0
#define ACTION_PRINT_PAYMENT_CHECK 1
#define ACTION_ORDER_CLOSE 100
#define ACTION_ORDER_INVALIDATE 101
#define ACTION_ORDER_MOVE 102
#define ACTION_RECIPE_ON_FLY 200

#define ROLE_VIEW_SALE_DOCS 4
#define ROLE_DEL_SALE_DOCS 5

struct Connection {
    char PathToDb [255];
    char Username [64];
    char Password [64];
    char SettingsPassword[64];
};

class QEDatabase  {
private:
    int m_dbNumber;
    QSqlDatabase m_db;
    static int Count;

public:
    QEDatabase();
    ~QEDatabase();
    void setDBParams(const QString &path, const QString &username, const QString &password);
    void setDBParams(const Connection &connection);
    bool open();
    void close();
    bool beginTransaction();
    bool commitTransaction();
    QSqlQuery *query();
    bool exec(QSqlQuery *q, const QString &sql);
    bool exec(QSqlQuery *q);
    bool prepare(QSqlQuery *q, const QString &sql);
};

struct User {
    int Id;
    int RoleId;
    QString Name;
    QVector<int> Roles;
    User () : Id(0), Name(""){}
    User &operator = (const User &user);
};

class QUser {
private:
    QEDatabase &m_db;

public:
    QUser(QEDatabase &db);
    User m_user;
    bool IdentifyByPassword(const QString &pass);
    QUser &operator = (const QUser &user);
};

struct Hall {
    int Id;
    QString Name;
    Hall () : Id(0), Name("") {}
    Hall (const int id, const QString name) : Id(id), Name(name) {}
};

struct Table {
    int Id;
    Hall hall;
    QString Name;
    int Queue;
    Table() : Id(0), Name(""), Queue(0) {}
    Table &operator = (const Table &table);
};

struct DishMenu {
    int Id;
    QString Name;
    DishMenu &operator = (const DishMenu &menu);
};

struct DishGroup {
    int Id;
    QString Name;
    DishGroup &operator = (const DishGroup &group);
};

class Dish {
private:
    friend class QOrder;
    QEDatabase m_db;
    double Quantity;
    double PrintedQuantity;
    void saveRecord();
    void setDefaultValues();

public:
    QString RecordId;
    QString OrderId;
    int StoreId;
    int Id;
    DishMenu Menu;
    DishGroup Group;
    QString Name;
    double Price;
    int printSchema;
    double quantity();
    double printed();
    void setQuantity(double value);
    void setPrintedQuantity(double value);
    bool isPrinted();
    Dish ();
    Dish (QEDatabase &db, DishMenu &menu, DishGroup &group);
    Dish &operator = (const Dish &dish);
    void genRecordId();
    void save();
    void deleteSelf();
};

class QOrder {
private:
    QEDatabase m_db;

public:
    QOrder(QEDatabase &db);
    QString Id;
    int m_status;
    QDate m_dateCash;
    QUser m_OrderOwner;
    QUser m_CurrentWaiter;
    Table m_table;
    double m_Amount;
    QMap<QString, Dish *> dishes;
    QMap<int, QVariant> mods;

    void dishesAmount();
    void newOrder();
    void saveOrder();
    void setDateCash(const QTime &time);
    void loadOrder(const QString &order_id);

    double serviceMarginValue();
    QVariant prop(const int prop_id);
    double prop_d(const int prop_id);
    void setProp_D(const int prop_id, const double &value);
    int prop_i(const int prop_id);
    void setProp_I(const int prop_id, const int &value);
};

namespace Data {

    extern QString fileConnection;
    extern QString computerName;
    extern QString fileLogo;
    extern QMap<int, QVariant> settings;
    extern QMap<int, Hall> halls;
    extern QMap<int, Table> tables;
    extern QMap<int, DishMenu> dishesMenus;
    extern QMap<int, DishGroup> dishesGroups;
    extern QMap<int, Dish> dishes;
    extern QMap<int, QVector<QString> > printSchema;

    void initHall(QSqlQuery *q);
    void initMenu(QEDatabase *db);
}


#endif // GLOBAL_H
