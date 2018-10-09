#include "ldishes.h"
#include "edish.h"
#include "fdish.h"
#include "erecipe.h"
#include "egroupchange.h"

LDishes::LDishes(QWidget *parent) : GridWindow(parent)
{
    setWindowTitle(tr("Dishes"));
    m_fmenu = "";
    m_fgroup = "";
    m_fvisible = true;
    topToolBar->addAction(QIcon(":icons/recipe.png"), tr("Recipe"), this, SLOT(slotRecipe()));
    topToolBar->addAction(QIcon(":/icons/group_event.png"), tr("Group event"), this, SLOT(slotGroupEvent()));
    getData();
}

void LDishes::getData()
{
    QString sql = "SELECT M.NAME AS MNAME, G.NAME AS GNAME, D.ID, D.MENU_ID, D.GROUP_ID, D.NAME, "
            "D.PRICE, D.STORE_ID, S.NAME AS SNAME, D.VISIBLE, D.PRINTER_ID, P.NAME AS PNAME, "
            "D.COLOR, D.QUEUE, D.COMBO, D.PRINT_QUEUE "
            "FROM DISHES D, DISHES_GROUPS G, MENU M, STORAGE S, PRINTERS P "
            "WHERE D.MENU_ID=M.ID AND D.GROUP_ID=G.ID AND D.STORE_ID=S.ID AND P.ID=D.PRINTER_ID ";

    if (m_fmenu.length() > 0)
        sql += QString(" AND D.MENU_ID IN (%1) ").arg(m_fmenu);

    if (m_fgroup.length() > 0)
        sql += QString(" AND D.GROUP_ID IN (%1) ").arg(m_fgroup);

    if (m_fstore.length() > 0)
        sql += QString(" AND D.STORE_ID IN (%1) ").arg(m_fstore);

    sql += QString(" AND D.VISIBLE=%1 ").arg(m_fvisible ? 1 : 0);
    sql += "ORDER BY QUEUE ";
    table->ExecuteQuery(sql);

    setColumnCaption("MNAME", tr("Menu"));
    setColumnCaption("GNAME", tr("Group"));
    setColumnCaption("ID", tr("Code"));
    setColumnCaption("MENU_ID", tr("Menu code"));
    setColumnCaption("GROUP_ID", tr("Group code"));
    setColumnCaption("NAME", tr("Name"));
    setColumnCaption("PRICE", tr("Price"));
    setColumnCaption("STORE_ID", tr("Store code"));
    setColumnCaption("SNAME", tr("Storage"));
    setColumn("PRINTER_ID", tr("Printer code"), 0, 496);
    setColumn("PNAME", tr("Printer"), 100, 0);
    setColumnCaption("VISIBLE", tr("Visible"));
    setColumn("COLOR", tr("Color"), 0, 496);
    setColumn("QUEUE", tr("Queue"), 0, 496);

    setColumnWidth("MNAME", 200);
    setColumnWidth("GNAME", 200);
    setColumnWidth("ID", 100);
    setColumnWidth("MENU_ID", 0);
    setColumnWidth("GROUP_ID", 0);
    setColumnWidth("NAME", 300);
    setColumnWidth("PRICE", 200);
    setColumnWidth("STORE_ID", 0);
    setColumnWidth("SNAME", 200);
    setColumnWidth("VISIBLE", 0);
    setColumnWidth("PRINT_QUEUE", 0);

    setColumnDataType("ID", 496);
    setColumnDataType("MENU_ID", 496);
    setColumnDataType("GROUP_ID", 496);
    setColumnDataType("STORE_ID", 496);
    setColumnDataType("VISIBLE", 496);
    setColumn("COMBO", tr("Combo"), 0, DATA_INTEGER);
    setColumn("PRINT_QUEUE", tr("Print queue"), 0, DATA_INTEGER);
}

void LDishes::setData(const EDish *e, const int row)
{
    table->m_model.setData(row, "MNAME", e->pedt_Menu->text());
    table->m_model.setData(row, "MENU_ID", e->m_menu);
    table->m_model.setData(row, "GNAME", e->pedt_Group->text());
    table->m_model.setData(row, "GROUP_ID", e->m_group);
    table->m_model.setData(row, "ID", e->pedt_Id->text());
    table->m_model.setData(row, "NAME", e->pedt_Name->text());
    table->m_model.setData(row, "PRICE", e->pedt_Price->text());
    table->m_model.setData(row, "SNAME", e->pedt_Store->text());
    table->m_model.setData(row, "STORE_ID", e->m_store);
    table->m_model.setData(row, "PRINTER_ID", e->m_printer);
    table->m_model.setData(row, "PNAME", e->pedt_Printer->text());
    table->m_model.setData(row, "VISIBLE", (e->pchk_Visible->isChecked() ? 1 : 0));
    table->m_model.setData(row, "COLOR", e->m_color->rgb());
    table->m_model.setData(row, "COMBO", (e->m_pchkCombo->isChecked() ? 1 : 0));
    table->m_model.setData(row, "PRINT_QUEUE", e->m_pPrintQueue->text().toInt());
}

void LDishes::setAdditionalData(const EDish *e)
{
    QSqlQuery *q = table->openDB();
    q->prepare("DELETE FROM DISHES_COMBO WHERE DISH_ID=:DISH_ID");
    q->bindValue(":DISH_ID", e->pedt_Id->text());
    q->exec();

    if (e->m_pchkCombo->isChecked())
    {
        for (int i = 0; i < e->m_ptCombo->m_model.rowCount(QModelIndex()); i++)
        {
            q->prepare("INSERT INTO DISHES_COMBO (DISH_ID, COMPONENT_ID, QTY, PRICE, PRINTER_ID) VALUES (:DISH_ID, :COMPONENT_ID, :QTY, :PRICE, :PRINTER_ID)");
            q->bindValue(":DISH_ID", e->pedt_Id->text());
            q->bindValue(":COMPONENT_ID", e->m_ptCombo->m_model.Integer("DISH_ID", i));
            q->bindValue(":QTY", e->m_ptCombo->m_model.Double("QTY", i));
            q->bindValue(":PRICE", e->m_ptCombo->m_model.Double("PRICE", i));
            q->bindValue(":PRINTER_ID", e->m_ptCombo->m_model.Integer("PRINTER_ID", i));
            q->exec();
        }
    }

    q->prepare("DELETE FROM DISHES_MODIFIERS WHERE DISH_ID=:DISH_ID");
    q->bindValue(":DISH_ID", e->pedt_Id->text());
    q->exec();

    for (int i = 0; i < e->m_ptModifiers->m_model.rowCount(QModelIndex()); i++)
    {
        q->prepare("INSERT INTO DISHES_MODIFIERS (DISH_ID, MODIFIER_ID) VALUES (:DISH_ID, :MODIFIER_ID)");
        q->bindValue(":DISH_ID", e->pedt_Id->text());
        q->bindValue(":MODIFIER_ID", e->m_ptModifiers->m_model.Integer("ID", i));
        q->exec();
    }
    table->closeDB(q);
}

void LDishes::slotNew()
{
    QSqlQuery *query = table->openDB();
    if (query == 0)
        return;

    EDish *e = new EDish(this);
    if (e->exec() == QDialog::Accepted)
    {
        if (exec(query, "SELECT GEN_ID(GEN_DISHES_ID, 1) FROM RDB$DATABASE"))
        {
            query->next();
            e->pedt_Id->setText(query->value(0).toString());
            if (prepare(query, "INSERT INTO DISHES (ID, MENU_ID, GROUP_ID, NAME, PRICE, STORE_ID, PRINTER_ID, VISIBLE, COLOR, QUEUE, COMBO, PRINT_QUEUE) VALUES "
                        "(:ID, :MENU_ID, :GROUP_ID, :NAME, :PRICE, :STORE_ID, :PRINTER_ID, :VISIBLE, :COLOR, :QUEUE, :COMBO, :PRINT_QUEUE)"))
            {
                query->bindValue(":ID", e->pedt_Id->text());
                query->bindValue(":MENU_ID", e->m_menu);
                query->bindValue(":GROUP_ID", e->m_group);
                query->bindValue(":NAME", e->pedt_Name->text());
                query->bindValue(":PRICE", e->pedt_Price->text());
                query->bindValue(":STORE_ID", e->m_store);
                query->bindValue(":PRINTER_ID", e->m_printer);
                query->bindValue(":VISIBLE", (e->pchk_Visible->isChecked() ? 1: 0));
                query->bindValue(":COLOR", e->m_color->rgb());
                query->bindValue(":QUEUE", e->pedt_Queue->text());
                query->bindValue(":COMBO", (e->m_pchkCombo->isChecked() ? 1 : 0));
                query->bindValue(":PRINT_QUEUE", e->m_pPrintQueue->text().toInt());
                if (exec(query))
                {
                    setAdditionalData(e);
                    if (m_fvisible == e->pchk_Visible->isChecked())
                        setData(e, table->addRow());
                }
            }
        }
    }
    delete e;
    table->closeDB(query);
}

void LDishes::slotEdit()
{
    QVector<int> row = getSelectedRows();
    if (row.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }
    if (row.count() > 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }

    QSqlQuery *query = table->openDB();
    if (query == 0)
        return;

    EDish *e = new EDish(this);
    e->pedt_Id->setText(table->m_model.data("ID", row[0]).toString());
    e->pedt_Menu->setText(table->m_model.data("MNAME", row[0]).toString());
    e->m_menu = table->m_model.data("MENU_ID", row[0]).toString();
    e->pedt_Group->setText(table->m_model.data("GNAME", row[0]).toString());
    e->m_group = table->m_model.String("GROUP_ID", row[0]);
    e->pedt_Name->setText(table->m_model.data("NAME", row[0]).toString());
    e->pedt_Price->setText(table->m_model.String("PRICE", row[0]));
    e->pedt_Store->setText(table->m_model.data("SNAME", row[0]).toString());
    e->m_printer = table->m_model.String("PRINTER_ID", row[0]);
    e->pedt_Printer->setText(table->m_model.data("PNAME", row[0]).toString());
    e->m_store = table->m_model.String("STORE_ID", row[0]);
    e->pchk_Visible->setChecked(table->m_model.data("VISIBLE", row[0]).toInt() == 1 ? true : false);
    *(e->m_color) = table->m_model.Integer("COLOR", row[0]);
    e->pedt_Queue->setText(table->m_model.String("QUEUE", row[0]));
    e->m_pPrintQueue->setText(table->m_model.String("PRINT_QUEUE", row[0]));
    e->setColor();
    e->m_pchkCombo->setChecked(table->m_model.data("COMBO", row[0]).toBool());
    e->slotCheckCombo(e->m_pchkCombo->isChecked());

    if (e->m_pchkCombo->isChecked())
    {
        query->prepare("SELECT D.ID, D.NAME, DC.QTY, DC.PRICE, DC.PRINTER_ID, P.NAME AS PNAME "
                       "FROM DISHES_COMBO DC, DISHES D, PRINTERS P "
                       "WHERE DC.COMPONENT_ID=D.ID AND DC.PRINTER_ID=P.ID AND DC.DISH_ID=:DISH_ID");
        query->bindValue(":DISH_ID", e->pedt_Id->text());
        query->exec();
        while (query->next())
        {
            int row = e->m_ptCombo->addRow();
            e->m_ptCombo->m_model.setData(row, "DISH_ID", query->value(0).toString());
            e->m_ptCombo->m_model.setData(row, "NAME", query->value(1).toString());
            e->m_ptCombo->m_model.setData(row, "QTY", query->value(2).toString());
            e->m_ptCombo->m_model.setData(row, "PRICE", query->value(3).toString());
            e->m_ptCombo->m_model.setData(row, "PRINTER_ID", query->value(4).toString());
            e->m_ptCombo->m_model.setData(row, "PRINTER", query->value(5).toString());
        }
    }

    query->prepare("SELECT DM.MODIFIER_ID, M.NAME AS MNAME "
                   "FROM DISHES_MODIFIERS DM, MODIFIERS M "
                   "WHERE DM.MODIFIER_ID=M.ID AND DM.DISH_ID=:DISH_ID");
    query->bindValue(":DISH_ID", e->pedt_Id->text());
    query->exec();
    while (query->next())
    {
        int row = e->m_ptModifiers->addRow();
        e->m_ptModifiers->m_model.setData(row, "ID", query->value(0).toInt());
        e->m_ptModifiers->m_model.setData(row, "NAME", query->value(1).toString());
    }

    if (e->exec() == QDialog::Accepted)
    {
        if (prepare(query,
                    "UPDATE DISHES SET MENU_ID=:MENU_ID, GROUP_ID=:GROUP_ID, NAME=:NAME, PRICE=:PRICE, "
                    "STORE_ID=:STORE_ID, PRINTER_ID=:PRINTER_ID, VISIBLE=:VISIBLE, COLOR=:COLOR, "
                    "QUEUE=:QUEUE, COMBO=:COMBO, PRINT_QUEUE=:PRINT_QUEUE WHERE ID=:ID"))
        {
            query->bindValue(":MENU_ID", e->m_menu);
            query->bindValue(":GROUP_ID", e->m_group);
            query->bindValue(":NAME", e->pedt_Name->text());
            query->bindValue(":PRICE", e->pedt_Price->text());
            query->bindValue(":STORE_ID", e->m_store);
            query->bindValue(":PRINTER_ID", e->m_printer);
            query->bindValue(":VISIBLE", (e->pchk_Visible->isChecked() ? 1 : 0));
            query->bindValue(":COLOR", e->m_color->rgb());
            query->bindValue(":QUEUE", e->pedt_Queue->text());
            query->bindValue(":COMBO", (e->m_pchkCombo->isChecked() ? 1 : 0));
            query->bindValue(":PRINT_QUEUE", e->m_pPrintQueue->text().toInt());
            query->bindValue(":ID", e->pedt_Id->text());
            if (exec(query))
            {
                setAdditionalData(e);
                if (e->pchk_Visible->isChecked() == m_fvisible)
                    setData(e, row[0]);
                else
                    table->m_model.removeRows(row[0], 1, QModelIndex());
            }
        }
    }
    delete e;
    table->closeDB(query);
}

void LDishes::slotFilter()
{
    FDish *f = new FDish(this);
    f->pedt_Menu->setText(m_fmenu);
    f->pedt_Group->setText(m_fgroup);
    f->pedt_Store->setText(m_fstore);
    f->pchk_Visible->setChecked(m_fvisible);
    if (f->exec() == QDialog::Accepted)
    {
        m_fmenu = f->pedt_Menu->text();
        m_fgroup = f->pedt_Group->text();
        m_fstore = f->pedt_Store->text();
        m_fvisible = f->pchk_Visible->isChecked();
        getData();
    }
    delete f;
}

void LDishes::slotRecipe()
{
    QVector<int> row = getSelectedRows();
    if (row.count() == 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }
    if (row.count() > 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }

    QSqlQuery *q = table->openDB();
    if (q == 0)
        return;

    ERecipe *e = new ERecipe(table->m_model.String("STORE_ID", row[0]), table->m_model.String("SNAME", row[0]), this);
    if (!table->m_model.Integer("COMBO", row[0]))
    {
        prepare(q, "SELECT R.GOODS_ID, G.NAME, U.ID AS UID, U.NAME AS UNAME, R.QTY, "
                "R.STORE_ID, S.NAME AS SNAME "
                "FROM RECIPES R, GOODS G, UNITS U, STORAGE S "
                "WHERE R.STORE_ID=S.ID AND R.GOODS_ID=G.ID AND G.UNIT_ID=U.ID AND R.DISH_ID=:DISH_ID ");
    }
    else
    {
        prepare(q, "SELECT R.GOODS_ID, G.NAME, U.ID AS UID, U.NAME AS UNAME, R.QTY, "
                "R.STORE_ID, S.NAME AS SNAME "
                "FROM RECIPES R, GOODS G, UNITS U, STORAGE S "
                "WHERE R.STORE_ID=S.ID AND R.GOODS_ID=G.ID AND G.UNIT_ID=U.ID "
                "AND R.DISH_ID IN (SELECT COMPONENT_ID FROM DISHES_COMBO WHERE DISH_ID=:DISH_ID) ");
    }

    q->bindValue(":DISH_ID", table->m_model.data("ID", row[0]).toString());
    exec(q);
    while (q->next())
    {
        int e_row = e->table->addRow();
        e->table->m_model.setData(e_row, "ID", q->value(0));
        e->table->m_model.setData(e_row, "NAME", q->value(1));
        e->table->m_model.setData(e_row, "UNIT_ID", q->value(2));
        e->table->m_model.setData(e_row, "UNAME", q->value(3));
        e->table->m_model.setData(e_row, "QTY", q->value(4));
        e->table->m_model.setData(e_row, "STORE_ID", q->value(5));
        e->table->m_model.setData(e_row, "STORE", q->value(6));
    }

    if (e->exec() == QDialog::Accepted)
    {
        if (prepare(q, "DELETE FROM RECIPES WHERE DISH_ID=:DISH_ID"))
        {
            q->bindValue(":DISH_ID", table->m_model.data("ID", row[0]).toString());
            exec(q);
        }

        if (prepare(q, "INSERT INTO RECIPES (DISH_ID, GOODS_ID, QTY, STORE_ID) VALUES (:DISH_ID, :GOODS_ID, :QTY, :STORE_ID)"))
        {
            q->bindValue(":DISH_ID", table->m_model.data("ID", row[0]).toString());
            for (int i = 0; i < e->table->m_model.rowCount(QModelIndex()); i++)
            {
                q->bindValue(":GOODS_ID", e->table->m_model.Integer("ID", i));
                q->bindValue(":QTY", e->table->m_model.Float("QTY", i));
                q->bindValue(":STORE_ID", e->table->m_model.String("STORE_ID", i));
                exec(q);
            }
        }

    }
    delete e;
    table->closeDB(q);
}

void LDishes::slotGroupEvent()
{
    EGroupChange *e = new EGroupChange(this);
    e->m_pVisible->setChecked(m_fvisible);
    if (e->exec() == QDialog::Accepted)
    {
        QString update_fields = "";
        if (e->m_menu.length())
            update_fields += "MENU_ID=" + e->m_menu + ",";
        if (e->m_group.length())
            update_fields += "GROUP_ID=" + e->m_group + ",";
        if (e->m_store.length())
            update_fields += "STORE_ID=" + e->m_store + ",";
        if (e->m_print.length())
            update_fields += "PRINTER_ID" + e->m_print + ",";
        if (e->m_pQueue->text().toInt() > 0)
            update_fields += "QUEUE=" + e->m_pQueue->text() + ",";

        QColor c;
        if (e->m_pColor->text().length())
        {
            c.setNamedColor(e->m_pColor->text());
            update_fields += "COLOR=:COLOR,";
        }

        update_fields += QString("VISIBLE=") + (e->m_pVisible->isChecked() ? "1" : "0") + ",";
        update_fields = update_fields.remove(update_fields.length() - 1, 1);
        QSqlQuery * q = table->openDB();
        for (int i = 0; i < table->m_model.rowCount(QModelIndex()); i++)
        {
            q->prepare("UPDATE DISHES SET " + update_fields + " WHERE ID=:ID");
            if (e->m_pColor->text().length())
                q->bindValue(":COLOR", c.rgb());
            q->bindValue(":ID", table->m_model.Integer("ID", i));
            q->exec();
        }
        table->closeDB(q);
        slotRefresh();
    }
    delete e;
}
