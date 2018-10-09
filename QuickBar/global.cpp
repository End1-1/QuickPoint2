#include "global.h"
#include "qbasedialog.h"

QString Data::fileConnection = "";
QString Data::computerName = "";
QString Data::fileLogo;
QMap<int, QVariant> Data::settings;
QMap<int, Hall> Data::halls;
QMap<int, Table> Data::tables;
QMap<int, DishMenu> Data::dishesMenus;
QMap<int, DishGroup> Data::dishesGroups;
QMap<int, Dish> Data::dishes;
QMap<int, QVector<QString> > Data::printSchema;

void Data::initHall(QSqlQuery *q)
{
    //Hall id, Hall name, Table id, Table name, Table queue
    Data::halls.clear();
    Data::tables.clear();
    while (q->next())
    {
        if (!Data::halls.contains(q->value(0).toInt()))
            Data::halls.insert(q->value(0).toInt(), Hall(q->value(0).toInt(), q->value(1).toString()));

        Table table;
        table.hall.Id = q->value(0).toInt();
        table.hall.Name = q->value(1).toString();
        table.Id = q->value(2).toInt();
        table.Name = q->value(3).toString();
        table.Queue = q->value(4).toInt();
        Data::tables.insert(q->value(2).toInt(), table);
    }
}

void Data::initMenu (QEDatabase *db)
{
    Data::dishesMenus.clear();
    Data::dishesGroups.clear();
    Data::dishes.clear();
    Data::printSchema.clear();

    QSqlQuery *q = db->query();
    QString sql = "SELECT PSD.SCHEMA_ID, PAD.PRINTER_NAME "
            "FROM PRINTER_ALIASES_DETAILS PAD, PRINTER_SCHEMA_DETAILS PSD "
            "WHERE PAD.ALIAS_PRINTER_ID=PSD.PRINTER_ID "
            "AND PAD.ALIAS_SCHEMA_ID=:ALIAS_SCHEMA_ID";

    q->prepare(sql);
    q->bindValue("ALIAS_SCHEMA_ID", Data::settings[S_PRINTER_ALIAS]);
    q->exec();
    while (q->next())
    {
        if (!Data::printSchema.contains(q->value(0).toInt()))
            printSchema.insert(q->value(0).toInt(), QVector<QString>());

        Data::printSchema[q->value(0).toInt()].push_back(q->value(1).toString());
    }

    sql = "SELECT M.ID AS MENU_ID, M.NAME AS MENU_NAME, DG.ID AS GROUP_ID, DG.NAME AS GROUP_NAME, "
            "D.ID AS DISH_ID, D.NAME AS DISH_NAME, D.PRICE, D.STORE_ID, D.PRINTER_ID "
            "FROM DISHES D, DISHES_GROUPS DG, MENU M "
            "WHERE DG.ID=D.GROUP_ID AND D.MENU_ID=M.ID AND VISIBLE=1 "
            "ORDER BY M.ID, DG.NAME, D.NAME ";

    db->exec(q, sql);
    while (q->next())
    {
        if (!dishesMenus.contains(q->value(0).toInt()))
        {
            DishMenu menu;
            menu.Id = q->value(0).toInt();
            menu.Name = q->value(1).toString();
            dishesMenus.insert(menu.Id, menu);
        }

        if (!dishesGroups.contains(q->value(2).toInt()))
        {
            DishGroup group;
            group.Id = q->value(2).toInt();
            group.Name = q->value(3).toString();
            dishesGroups.insert(group.Id, group);
        }

        Dish dish(*db, dishesMenus[q->value(0).toInt()], dishesGroups[q->value(2).toInt()]);
        dish.Id = q->value(4).toInt();
        dish.Name = q->value(5).toString();
        dish.Price = q->value(6).toDouble();
        dish.StoreId = q->value(7).toInt();
        dish.printSchema = q->value(8).toInt();
        dishes.insert(dish.Id, dish);
    }
    delete q;
}


Dish::Dish()
{
    setDefaultValues();
}

Dish::Dish(QEDatabase &db, DishMenu &menu, DishGroup &group) :
    m_db(db), Menu(menu), Group(group)
{
    setDefaultValues();
}

Dish &Dish::operator =(const Dish &dish)
{
    m_db = dish.m_db;
    RecordId = "";
    OrderId = "";
    StoreId = dish.StoreId;
    Id = dish.Id;
    Menu = dish.Menu;
    Group = dish.Group;
    Name = dish.Name;
    Price = dish.Price;
    Quantity = dish.Quantity;
    PrintedQuantity = dish.PrintedQuantity;
    printSchema = dish.printSchema;
    return *this;
}

void Dish::genRecordId()
{
    m_db.open();
    QSqlQuery *q = m_db.query();
    m_db.exec(q, "SELECT GEN_ID(GEN_SALE_BODY_ID, 1) FROM RDB$DATABASE");
    q->next();
    RecordId = Data::settings[S_ORDER_ID_PREFIX].toString() + q->value(0).toString();
    m_db.prepare(q, "INSERT INTO SALE_BODY (ID) VALUES (:ID)");
    q->bindValue("ID", RecordId);
    m_db.exec(q);
    delete q;
    m_db.close();
    save();
}

void Dish::save()
{
    QString sql = "UPDATE SALE_BODY SET ORDER_ID=:ORDER_ID, DISH_ID=:DISH_ID, PRICE=:PRICE, QTY=:QTY, PRINTED=:PRINTED, STORE_ID=:STORE_ID "
            "WHERE ID=:ID";
    m_db.open();
    QSqlQuery *q = m_db.query();
    q->prepare(sql);
    q->bindValue("ORDER_ID", OrderId);
    q->bindValue("DISH_ID", Id);
    q->bindValue("PRICE", Price);
    q->bindValue("QTY", Quantity);
    q->bindValue("PRINTED", PrintedQuantity);
    q->bindValue("STORE_ID", StoreId);
    q->bindValue("ID", RecordId);
    m_db.exec(q);
    delete q;
    m_db.close();
}

void Dish::deleteSelf()
{
    QString sql = "DELETE FROM SALE_BODY WHERE ID=:ID";
    m_db.open();
    QSqlQuery *q = m_db.query();
    m_db.prepare(q, sql);
    q->bindValue("ID", RecordId);
    m_db.exec(q);
    delete q;
    m_db.close();
}


DishMenu &DishMenu::operator =(const DishMenu &menu)
{
    Id = menu.Id;
    Name = menu.Name;
    return *this;
}


DishGroup &DishGroup::operator =(const DishGroup &group)
{
    Id = group.Id;
    Name = group.Name;
    return *this;
}


Table &Table::operator =(const Table &table)
{
    Id = table.Id;
    Name = table.Name;
    hall.Id = table.hall.Id;
    hall.Name = table.hall.Name;
    Queue = table.Queue;
    return *this;
}


User &User::operator =(const User &user)
{
    Id = user.Id;
    Name = user.Name;
    RoleId = user.RoleId;
    Roles = user.Roles;
    return *this;
}


void Dish::saveRecord()
{
    m_db.open();
    QSqlQuery *q = m_db.query();
    m_db.prepare(q, "UPDATE SALE_BODY SET PRICE=:PRICE, QTY=:QTY, PRINTED=:PRINTED WHERE ID=:ID");
    q->bindValue("PRICE", Price);
    q->bindValue("QTY", Quantity);
    q->bindValue("PRINTED", PrintedQuantity);
    q->bindValue("ID", RecordId);
    m_db.exec(q);
    m_db.close();
    delete q;
}

void Dish::setDefaultValues()
{
    Quantity = 1;
    PrintedQuantity = 0;
}

double Dish::quantity()
{
    return Quantity;
}

double Dish::printed()
{
    return PrintedQuantity;
}

void Dish::setQuantity(double value)
{
    Quantity = value;
    saveRecord();
}

void Dish::setPrintedQuantity(double value)
{
    PrintedQuantity = value;
    saveRecord();
}

bool Dish::isPrinted()
{
    return Quantity == PrintedQuantity;
}

QEDatabase::QEDatabase()
{
    m_dbNumber = Count++;
    m_db = QSqlDatabase::addDatabase(DBTYPE, QString::number(m_dbNumber));
}

QEDatabase::~QEDatabase()
{
//    QSqlDatabase::removeDatabase(QString::number(m_dbNumber));
}

void QEDatabase::setDBParams(const QString &path, const QString &username, const QString &password)
{
    m_db.setDatabaseName(path);
    m_db.setUserName(username);
    m_db.setPassword(password);
    m_db.setConnectOptions("WaitOnLocks=false;");
}

void QEDatabase::setDBParams(const Connection &connection)
{
    setDBParams(connection.PathToDb, connection.Username, connection.Password);
}

bool QEDatabase::open()
{
    if (m_db.open())
        return true;
    else
    {
        QMessageBox::critical(0, "Database error", m_db.lastError().databaseText());
        return false;
    }
}

QOrder::QOrder(QEDatabase &db) :
    m_db(db), m_OrderOwner(db), m_CurrentWaiter(db)
{
    m_Amount = 0;
    Id = "";
    m_status = ORDER_OPEN;
    m_dateCash = QDate::currentDate();
}

void QOrder::dishesAmount()
{
    mods[ORDER_MOD_COUNTED] = 0;
    for (QMap<QString, Dish *>::iterator i = dishes.begin(); i != dishes.end(); i++)
        mods[ORDER_MOD_COUNTED] = mods[ORDER_MOD_COUNTED].toDouble() + (i.value()->quantity() * i.value()->Price);
    mods[ORDER_MOD_SERVICE_AMOUNT] = mods[ORDER_MOD_COUNTED].toDouble() * (mods[ORDER_MOD_SERVICE_VALUE].toDouble() / 100);
    m_Amount = mods[ORDER_MOD_COUNTED].toDouble() + mods[ORDER_MOD_SERVICE_AMOUNT].toDouble();
}

void QOrder::newOrder()
{
    m_db.open();
    QSqlQuery *q = m_db.query();
    m_db.exec(q, "SELECT GEN_ID(GEN_SALE_HEADER_ID, 1) FROM RDB$DATABASE");
    q->next();
    Id = Data::settings[S_ORDER_ID_PREFIX].toString() + q->value(0).toString();

    m_db.prepare(q, "INSERT INTO SALE_HEADER (ID) VALUES (:ID)");
    q->bindValue("ID", Id);
    m_db.exec(q);
    m_db.close();
    delete q;

    mods.insert(ORDER_MOD_SERVICE_VALUE, Data::settings[S_SERVICE_MARGIN].toDouble());
    mods.insert(ORDER_MOD_SERVICE_AMOUNT, 0);
    mods.insert(ORDER_MOD_TIME_OPEN, QDateTime::currentDateTime().toString(DATETIME_FORMAT));
}

void QOrder::saveOrder()
{
    m_db.open();
    QSqlQuery *q = m_db.query();
    QString sql = "UPDATE SALE_HEADER SET STATUS_ID=:STATUS_ID, DATE_CASH=:DATE_CASH, "
            "TABLE_ID=:TABLE_ID, WAITER_ID=:WAITER_ID, AMOUNT=:AMOUNT "
            "WHERE ID=:ID";
    m_db.prepare(q, sql);
    q->bindValue("STATUS_ID", m_status);
    q->bindValue("DATE_CASH", m_dateCash);
    q->bindValue("TABLE_ID", m_table.Id);
    q->bindValue("WAITER_ID", m_OrderOwner.m_user.Id);
    q->bindValue("AMOUNT", m_Amount);
    q->bindValue("ID", Id);
    m_db.exec(q);

    m_db.prepare(q, "DELETE FROM SALE_MODS WHERE ORDER_ID=:ORDER_ID");
    q->bindValue("ORDER_ID", Id);
    m_db.exec(q);

    for (QMap<int, QVariant>::const_iterator i = mods.begin(); i != mods.end(); i++)
    {
        m_db.prepare(q, "INSERT INTO SALE_MODS (ORDER_ID, MOD_ID, DATA) VALUES (:ORDER_ID, :MOD_ID, :DATA);");
        q->bindValue("ORDER_ID", Id);
        q->bindValue("MOD_ID", i.key());
        q->bindValue("DATA", *i);
        m_db.exec(q);
    }

    delete q;
    m_db.close();
}

void QOrder::setDateCash(const QTime &time)
{
    if (QTime::currentTime() < time)
        m_dateCash = QDate::currentDate().addDays(-1);
    else
        m_dateCash = QDate::currentDate();
}

void QOrder::loadOrder(const QString &order_id)
{
    Id = order_id;
    if (!Id.length())
        return;

    m_db.open();
    QSqlQuery *q = m_db.query();
    QString sql = "SELECT SH.STATUS_ID, SH.DATE_CASH, "
            "H.ID AS HALL_ID, H.NAME AS HALL_NAME, "
            "SH.TABLE_ID, T.NAME AS TABLE_NAME, "
            "SH.WAITER_ID, E.NAME AS WAITER_NAME, "
            "SH.AMOUNT "
            "FROM SALE_HEADER SH, HALL H, HALL_TABLES T, EMPLOYEES E "
            "WHERE SH.TABLE_ID=T.ID AND T.HALL_ID=H.ID AND SH.WAITER_ID=E.ID AND SH.ID=:ID";
    m_db.prepare(q, sql);
    q->bindValue("ID", order_id);
    m_db.exec(q);
    q->next();
    int i = -1;
    m_status = q->value(++i).toInt();
    m_dateCash = q->value(++i).toDate();
    m_table.hall.Id = q->value(++i).toInt();
    m_table.hall.Name = q->value(++i).toString();
    m_table.Id = q->value(++i).toInt();
    m_table.Name = q->value(++i).toString();
    m_OrderOwner.m_user.Id = q->value(++i).toInt();
    m_OrderOwner.m_user.Name = q->value(++i).toString();

    sql = "SELECT MOD_ID, DATA FROM SALE_MODS WHERE ORDER_ID=:ORDER_ID";
    m_db.prepare(q, sql);
    q->bindValue("ORDER_ID", Id);
    m_db.exec(q);
    while (q->next())
        mods.insert(q->value(0).toInt(), q->value(1));

    sql = "SELECT SB.ID, "
            "M.ID AS MENU_ID, M.NAME AS MENU_NAME, "
            "DG.ID AS DISH_GROUP_ID, DG.NAME AS DISH_GROUP_NAME, "
            "SB.DISH_ID, D.NAME AS DISH_NAME,  "
            "SB.QTY, SB.PRINTED, SB.PRICE, SB.STORE_ID,D.PRINTER_ID  "
            "FROM SALE_BODY SB, DISHES D, MENU M, DISHES_GROUPS DG "
            "WHERE SB.DISH_ID=D.ID AND SB.ORDER_ID=:ORDER_ID AND M.ID=D.MENU_ID AND D.GROUP_ID=DG.ID ";
    m_db.prepare(q, sql);
    q->bindValue("ORDER_ID", order_id);
    m_db.exec(q);
    while (q->next())
    {

        DishMenu menu;
        menu.Id = q->value(1).toInt();
        menu.Name = q->value(2).toString();
        DishGroup group;
        group.Id = q->value(3).toInt();
        group.Name = q->value(4).toString();
        Dish *dish = new Dish(m_db, menu, group);
        dish->RecordId = q->value(0).toString();
        dish->Id = q->value(5).toInt();
        dish->Name = q->value(6).toString();
        dish->Quantity = q->value(7).toDouble();
        dish->PrintedQuantity = q->value(8).toDouble();
        dish->Price = q->value(9).toDouble();
        dish->StoreId = q->value(10).toInt();
        dish->printSchema = q->value(11).toInt();

        dishes.insert(dish->RecordId, dish);
    }
    m_db.close();
    delete q;

    dishesAmount();
}

double QOrder::serviceMarginValue()
{
    if (mods.contains(ORDER_MOD_SERVICE_VALUE))
        return mods[ORDER_MOD_SERVICE_VALUE].toDouble();
    else
        return 0;
}

QVariant QOrder::prop(const int prop_id)
{
    return mods[prop_id];
}

double QOrder::prop_d(const int prop_id)
{
    return mods[prop_id].toDouble();
}

void QOrder::setProp_D(const int prop_id, const double &value)
{
    mods[prop_id] = value;
}

int QOrder::prop_i(const int prop_id)
{
    return mods[prop_id].toInt();
}

void QOrder::setProp_I(const int prop_id, const int &value)
{
    mods[prop_id] = value;
}


QUser::QUser(QEDatabase &db) :
    m_db(db)
{

}

bool QUser::IdentifyByPassword(const QString &pass)
{
    QString password = pass.trimmed();
    if (!password.length())
        return false;

    QCryptographicHash h(QCryptographicHash::Md5);
    password = h.hash(password.toAscii(), QCryptographicHash::Md5).toBase64();

    QString sql = "SELECT E.ID, E.NAME, L.ROLE_ID "
            "FROM EMPLOYEES E, LOGINS L "
            "WHERE E.ID=L.EMPLOYEE_ID AND L.PASSWORD=:PASSWORD";

    m_db.open();
    QSqlQuery *q = m_db.query();
    m_db.prepare(q, sql);
    q->bindValue("PASSWORD", password);
    m_db.exec(q);
    if (q->next())
    {
        m_user.Id = q->value(0).toInt();
        m_user.Name = q->value(1).toString();
        m_user.RoleId = q->value(2).toInt();
    }

    sql = "SELECT ACTION_ID FROM ROLES_SET WHERE ROLE_ID=:ROLE_ID";
    if (m_db.prepare(q, sql))
    {
        q->bindValue("ROLE_ID", m_user.RoleId);
        m_db.exec(q);
        while (q->next())
            m_user.Roles.push_back(q->value(0).toInt());
    }

    delete q;
    m_db.close();
    if (m_user.Id > 0)
        return true;
    else
        return false;
}

QUser &QUser::operator =(const QUser &user)
{
    m_user = user.m_user;
    return *this;
}
