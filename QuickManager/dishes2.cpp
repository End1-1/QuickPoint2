#include "dishes2.h"
#include "edish.h"
#include "erecipe.h"
#include "egroupchange.h"
#include "fdish.h"

Dishes2::Dishes2(QWidget *parent) :
    GridWindow2(tr("Dishes"), parent)
{
    m_columns << "M.NAME AS MNAME" << "G.NAME AS GNAME" << "D.ID"
              << "D.MENU_ID" << "D.GROUP_ID" << "D.NAME" << "D.PRICE"
              << "D.STORE_ID" << "S.NAME AS SNAME" << "D.VISIBLE"
              << "D.PRINTER_ID" << "P.NAME AS PNAME"
              << "D.COLOR" << "D.QUEUE" << "D.COMBO" << "D.PRINT_QUEUE";

    m_headerCaptions << tr("Menu") << tr("Group") << tr("Id") << tr("Menu id")
                   << tr("Group id")  << tr("Name") << tr("Price") << tr("Store id")
                   << tr("Store") <<tr("Visible") << tr("Printer id") << tr("Printer")
                   << tr("Color") << tr("Queue") << tr("Combo") << tr("Print queue");

    m_headerWidths << 100 << 100 << 100 << 0 << 0 << 200 << 80 << 0 << 100
                    << 0 << 0 << 100 << 0 << 80 << 0 << 80;

    m_pToolBar->addAction(QIcon(":icons/recipe.png"), tr("Recipe"), this, SLOT(slotRecipe()));
    m_pToolBar->addAction(QIcon(":/icons/group_event.png"), tr("Group event"), this, SLOT(slotGroupEvent()));

    setDefaultFilter();
    slotRefresh();
}

void Dishes2::setDataFromDialog(const int &row, const QDialog *dialog)
{
    const EDish *e = dynamic_cast<const EDish *>(dialog);
    QVector<QVariant> d;
    d.push_back(e->pedt_Menu->text());
    d.push_back(e->pedt_Group->text());
    d.push_back(e->pedt_Id->text().toInt());
    d.push_back(e->m_menu);
    d.push_back(e->m_group);
    d.push_back(e->pedt_Name->text());
    d.push_back(e->pedt_Price->text().toDouble());
    d.push_back(e->m_store);
    d.push_back(e->pedt_Store->text());
    d.push_back((e->pchk_Visible->isChecked() ? 1 : 0));
    d.push_back(e->m_printer);
    d.push_back(e->pedt_Printer->text());
    d.push_back(e->m_color->rgb());
    d.push_back(e->pedt_Queue->text().toInt());
    d.push_back((e->m_pchkCombo->isChecked() ? 1 : 0));
    d.push_back(e->m_pPrintQueue->text().toInt());
    setData(row, d);
}

void Dishes2::slotNew()
{
    EDish *e = new EDish(this);
    if (e->exec() == QDialog::Accepted)
    {
        QMap<QString, QVariant> v;
        v.insert("ID", 0);
        v.insert("MENU_ID", e->m_menu);
        v.insert("GROUP_ID", e->m_group);
        v.insert("NAME", e->pedt_Name->text());
        v.insert("PRICE", e->pedt_Price->text().toDouble());
        v.insert("STORE_ID", e->m_store);
        v.insert("PRINTER_ID", e->m_printer);
        v.insert("VISIBLE", (e->pchk_Visible->isChecked() ? 1: 0));
        v.insert("COLOR", e->m_color->rgb());
        v.insert("QUEUE", e->pedt_Queue->text());
        v.insert("COMBO", (e->m_pchkCombo->isChecked() ? 1 : 0));
        v.insert("PRINT_QUEUE", e->m_pPrintQueue->text().toInt());
        if (insert("GEN_DISHES_ID", "DISHES", v) > -1) {
            e->pedt_Id->setText(v["ID"].toString());
            setAdditionalData(e);
            if (v["VISIBLE"].toBool() == e->pchk_Visible->isChecked()) {
                int row = m_pTable->rowCount();
                m_pTable->setRowCount(row + 1);
                for (int i = 0; i < m_headerCaptions.count(); i++) {
                    QTableWidgetItem *ti = new QTableWidgetItem();
                    ti->setData(Qt::DisplayRole, "");
                    ti->setBackgroundColor(e->m_color->rgb());
                    ti->setFlags(ti->flags() ^ Qt::ItemIsEditable);
                    m_pTable->setItem(row, i, ti);
                }
                setDataFromDialog(row, e);
            }
        }
    }
}

void Dishes2::slotEdit()
{

    QList<QTableWidgetItem *> ti = m_pTable->selectedItems();
    if (!ti.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }

    QList<int> rows;
    for (int i = ti.count() - 1; i > -1; i--) {
        if (rows.contains(ti[i]->row()))
            ti.removeAt(i);
        else
            rows.push_back(ti[i]->row());
    }

    if (ti.count() >  1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }

    QMap<QString, QVariant> v;
    getData(ti[0]->row(), v);
    if (!v.count())
        return;

    EDish *e = new EDish(this);
    e->pedt_Id->setText(v["D.ID"].toString());
    e->pedt_Menu->setText(v["M.NAME AS MNAME"].toString());
    e->m_menu = v["D.MENU_ID"].toString();
    e->pedt_Group->setText(v["G.NAME AS GNAME"].toString());
    e->m_group = v["D.GROUP_ID"].toString();
    e->pedt_Name->setText(v["D.NAME"].toString());
    e->pedt_Price->setText(v["D.PRICE"].toString());
    e->pedt_Store->setText(v["S.NAME AS SNAME"].toString());
    e->m_printer = v["D.PRINTER_ID"].toString();
    e->pedt_Printer->setText(v["P.NAME AS PNAME"].toString());
    e->m_store = v["D.STORE_ID"].toString();
    e->pchk_Visible->setChecked(v["D.VISIBLE"].toInt() == 1 ? true : false);
    *(e->m_color) = v["D.COLOR"].toInt();
    e->pedt_Queue->setText(v["D.QUEUE"].toString());
    e->m_pPrintQueue->setText(v["D.PRINT_QUEUE"].toString());
    e->setColor();
    e->m_pchkCombo->setChecked(v["D.COMBO"].toBool());
    e->slotCheckCombo(e->m_pchkCombo->isChecked());

    m_db.open();
    QSqlQuery q(m_db);
    QStringList group;
    if (e->m_pchkCombo->isChecked())
    {
        if (query(q, "DISHES_COMBO DC, DISHES D, PRINTERS P",
              QStringList () << "D.ID" << "D.NAME" << "DC.QTY" << "DC.PRICE" << "DC.PRINTER_ID" <<  "P.NAME AS PNAME",
              QStringList () << "DC.COMPONENT_ID=D.ID" << "DC.PRINTER_ID=P.ID" << "DC.DISH_ID=" + e->pedt_Id->text(), group)) {
                while (q.next()) {
                    int row = e->m_ptCombo->addRow();
                    e->m_ptCombo->m_model.setData(row, "DISH_ID", q.value(0).toString());
                    e->m_ptCombo->m_model.setData(row, "NAME", q.value(1).toString());
                    e->m_ptCombo->m_model.setData(row, "QTY", q.value(2).toString());
                    e->m_ptCombo->m_model.setData(row, "PRICE", q.value(3).toString());
                    e->m_ptCombo->m_model.setData(row, "PRINTER_ID", q.value(4).toString());
                    e->m_ptCombo->m_model.setData(row, "PRINTER", q.value(5).toString());
                }
        }
    }

    if (query(q, "DISHES_MODIFIERS DM, MODIFIERS M",
              QStringList() << "DM.MODIFIER_ID" <<  "M.NAME AS MNAME",
              QStringList() << "DM.MODIFIER_ID=M.ID" << "DM.DISH_ID=" + e->pedt_Id->text(),
              group)) {
        while (q.next())
        {
            int row = e->m_ptModifiers->addRow();
            e->m_ptModifiers->m_model.setData(row, "ID", q.value(0).toInt());
            e->m_ptModifiers->m_model.setData(row, "NAME", q.value(1).toString());
        }
    }

    if (e->exec() == QDialog::Accepted)
    {
        QMap<QString, QVariant> vu, where;
        vu.insert("MENU_ID", e->m_menu);
        vu.insert("GROUP_ID", e->m_group);
        vu.insert("NAME", e->pedt_Name->text());
        vu.insert("PRICE", e->pedt_Price->text());
        vu.insert("STORE_ID", e->m_store);
        vu.insert("PRINTER_ID", e->m_printer);
        vu.insert("VISIBLE", (e->pchk_Visible->isChecked() ? 1 : 0));
        vu.insert("COLOR", e->m_color->rgb());
        vu.insert("QUEUE", e->pedt_Queue->text());
        vu.insert("COMBO", (e->m_pchkCombo->isChecked() ? 1 : 0));
        vu.insert("PRINT_QUEUE", e->m_pPrintQueue->text().toInt());
        where.insert("ID", e->pedt_Id->text());
        update("DISHES", vu, where);
        setAdditionalData(e);

        vu.clear();
        vu.insert("STORE_ID", e->m_store);
        where.clear();
        where.insert("DISH_ID", e->pedt_Id->text());
        update("RECIPES", vu, where);

        //TODO:update color of cells

        if (e->pchk_Visible->isChecked() == m_sqlWhere.contains("D.VISIBLE=1"))
            setDataFromDialog(ti[0]->row(), e);
        else
            m_pTable->removeRow(ti[0]->row());

    }
    delete e;
    m_db.close();
}

void Dishes2::slotRefresh()
{
    QString tables = "DISHES D, DISHES_GROUPS G, MENU M, STORAGE S, PRINTERS P ";

    m_db.open();
    QSqlQuery q(m_db);
    query(q, tables, m_columns, m_sqlWhere, QStringList() << "G.NAME" << "D.NAME");

    m_pTable->setColumnCount(m_columns.count());
    m_pTable->setRowCount(500);

    if (q.next()) {
        int row = 0, col_count = m_headerCaptions.count();
        for (int i = 0; i < q.record().count(); i++)
            m_pTable->setColumnWidth(i, m_headerWidths.at(i));
        m_pTable->setHorizontalHeaderLabels(m_headerCaptions);

        do {
            if (row > m_pTable->rowCount() - 1)
                m_pTable->setRowCount(m_pTable->rowCount() + 500);

            for (int i = 0; i < col_count; i++) {
                QTableWidgetItem *ti = new QTableWidgetItem();
                ti->setData(Qt::DisplayRole, q.value(i));
                ti->setBackgroundColor(q.value(12).toUInt());
                ti->setFlags(ti->flags() ^ Qt::ItemIsEditable);
                m_pTable->setItem(row, i, ti);
            }
            row++;
        } while (q.next());

        m_pTable->setRowCount(row);
    } else {
        m_pTable->setRowCount(0);
    }

    m_db.close();
}

void Dishes2::slotFilter()
{
    FDish *f = new FDish(this);
    //TODO: set current filter parameters
    f->pchk_Visible->setChecked(m_sqlWhere.contains("D.VISIBLE=1"));
    if (f->exec() == QDialog::Accepted)
    {
        m_sqlWhere.clear();
        m_sqlWhere.push_back("D.MENU_ID=M.ID");
        m_sqlWhere.push_back("D.GROUP_ID=G.ID");
        m_sqlWhere.push_back("D.STORE_ID=S.ID");
        m_sqlWhere.push_back("P.ID=D.PRINTER_ID");

        if (f->pedt_Menu->text().length())
            m_sqlWhere.push_back("D.MENU_ID IN(" + f->pedt_Menu->text() + ")");

        if (f->pedt_Group->text().length())
            m_sqlWhere.push_back("D.GROUP_ID IN (" + f->pedt_Group->text() + ")");

        if (f->pedt_Store->text().length())
            m_sqlWhere.push_back("D.STORE_ID IN (" + f->pedt_Store->text() + ")");

        if (f->pchk_NoRecipe->isChecked())
            m_sqlWhere.push_back("D.ID NOT IN (SELECT DISH_ID FROM RECIPES)");

        m_sqlWhere.push_back("D.VISIBLE=" + (f->pchk_Visible->isChecked() ? QString("1") : QString("0")));
        slotRefresh();
    }
    delete f;
}

void Dishes2::setDefaultFilter()
{
    m_sqlWhere.push_back("D.VISIBLE=1");
    m_sqlWhere.push_back("D.MENU_ID=M.ID");
    m_sqlWhere.push_back("D.GROUP_ID=G.ID");
    m_sqlWhere.push_back("D.STORE_ID=S.ID");
    m_sqlWhere.push_back("P.ID=D.PRINTER_ID");
}

void Dishes2::setAdditionalData(const EDish *e)
{
    QMap<QString, QVariant> del_where;
    del_where.insert("DISH_ID", e->pedt_Id->text());
    del("DISHES_COMBO", del_where);

    if (e->m_pchkCombo->isChecked())
    {
        for (int i = 0; i < e->m_ptCombo->m_model.rowCount(QModelIndex()); i++)
        {
            QMap<QString, QVariant> values;
            values.insert("DISH_ID", e->pedt_Id->text());
            values.insert("COMPONENT_ID", e->m_ptCombo->m_model.Integer("DISH_ID", i));
            values.insert("QTY", e->m_ptCombo->m_model.Double("QTY", i));
            values.insert("PRICE", e->m_ptCombo->m_model.Double("PRICE", i));
            values.insert("PRINTER_ID", e->m_ptCombo->m_model.Integer("PRINTER_ID", i));
            insert("", "DISHES_COMBO", values);
        }
    }

    del("DISHES_MODIFIERS", del_where);

    for (int i = 0; i < e->m_ptModifiers->m_model.rowCount(QModelIndex()); i++)
    {
        QMap<QString, QVariant> values;
        values.insert("DISH_ID", e->pedt_Id->text());
        values.insert("MODIFIER_ID", e->m_ptModifiers->m_model.Integer("ID", i));
        insert("", "DISHES_MODIFIERS", values);
    }
}

void Dishes2::slotRecipe()
{
    QList<QTableWidgetItem *> ti = m_pTable->selectedItems();
    QVector<int> rows;
    for (QList<QTableWidgetItem *>::const_iterator i = ti.begin(); i != ti.end(); i++)
        if (!rows.contains((*i)->row()))
            rows.push_back((*i)->row());

    if (!rows.count())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nothing selected"));
        return;
    }
    if (rows.count() > 1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Multiple selection"));
        return;
    }

    ERecipe *e = new ERecipe(m_pTable->item(rows[0], 7)->text(), m_pTable->item(rows[0], 8)->text(), this);
    m_db.open();
    QSqlQuery q(m_db);

    if (!m_pTable->item(rows[0], 14)->text().toInt())
    {
        if (!q.prepare("SELECT R.GOODS_ID, G.NAME, U.ID AS UID, U.NAME AS UNAME, R.QTY, "
                "R.STORE_ID, S.NAME AS SNAME "
                "FROM RECIPES R, GOODS G, UNITS U, STORAGE S "
                "WHERE R.STORE_ID=S.ID AND R.GOODS_ID=G.ID AND G.UNIT_ID=U.ID AND R.DISH_ID=:DISH_ID "))
            QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
    }
    else
    {
        if(!q.prepare("SELECT R.GOODS_ID, G.NAME, U.ID AS UID, U.NAME AS UNAME, R.QTY, "
                "R.STORE_ID, S.NAME AS SNAME "
                "FROM RECIPES R, GOODS G, UNITS U, STORAGE S "
                "WHERE R.STORE_ID=S.ID AND R.GOODS_ID=G.ID AND G.UNIT_ID=U.ID "
                "AND R.DISH_ID IN (SELECT COMPONENT_ID FROM DISHES_COMBO WHERE DISH_ID=:DISH_ID) "))
            QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
    }

    q.bindValue(":DISH_ID", m_pTable->item(rows[0], 2)->text().toInt());
    if (!q.exec())
        QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
    while (q.next())
    {
        int e_row = e->table->addRow();
        e->table->m_model.setData(e_row, "ID", q.value(0));
        e->table->m_model.setData(e_row, "NAME", q.value(1));
        e->table->m_model.setData(e_row, "UNIT_ID", q.value(2));
        e->table->m_model.setData(e_row, "UNAME", q.value(3));
        e->table->m_model.setData(e_row, "QTY", q.value(4));
        e->table->m_model.setData(e_row, "STORE_ID", q.value(5));
        e->table->m_model.setData(e_row, "STORE", q.value(6));
    }

    m_db.close();
    if (e->exec() == QDialog::Accepted)
    {
        QMap<QString, QVariant> where;
        where.insert("DISH_ID", m_pTable->item(rows[0], 2)->text().toInt());
        del("RECIPES", where);

        for (int i = 0; i < e->table->m_model.rowCount(QModelIndex()); i++)
        {
            QMap<QString, QVariant> values;
            values.insert("DISH_ID", m_pTable->item(rows[0], 2)->text().toInt());
            values.insert("GOODS_ID", e->table->m_model.Integer("ID", i));
            values.insert("QTY", e->table->m_model.Float("QTY", i));
            values.insert("STORE_ID", e->table->m_model.String("STORE_ID", i));
            insert("", "RECIPES", values);
        }

    }

    delete e;
}

void Dishes2::slotGroupEvent()
{
    EGroupChange *e = new EGroupChange(this);
    e->m_pVisible->setChecked(m_sqlWhere.contains("D.VISIBLE=1"));
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
            update_fields += "PRINTER_ID=" + e->m_print + ",";
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

        m_db.open();
        QSqlQuery q(m_db);
        for (int i = 0; i < m_pTable->rowCount(); i++)
        {
            if (!q.prepare("UPDATE DISHES SET " + update_fields + " WHERE ID=:ID"))
                QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());

            if (e->m_pColor->text().length())
                q.bindValue(":COLOR", c.rgb());
            q.bindValue(":ID", m_pTable->item(i, 2)->text().toInt());
            q.exec();

            if (e->m_store.length()) {
                if (!q.prepare("UPDATE RECIPES SET STORE_ID=" + e->m_store + " WHERE DISH_ID=" + m_pTable->item(i, 2)->text()))
                    QMessageBox::critical(this, tr("Error"), q.lastError().databaseText());
                q.exec();
            }
        }

        m_db.close();
        slotRefresh();
    }
    delete e;
}

void Dishes2::slotDelete()
{
    QList<int> rows;
    if (!getSelectedRows(rows)) {
        QMessageBox::critical(this, tr("Error"), tr("Nothing is selected"));
        return;
    }
    std::sort(rows.begin(), rows.end());
    std::reverse(rows.begin(), rows.end());

    if (QMessageBox::question(this, tr("Warning"), tr("Confirm action"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    QList<int> not_deleted;
    for (QList<int>::const_iterator i = rows.begin(); i != rows.end(); i++) {
        m_db.open();
        QSqlQuery q(m_db);
        query(q, "SALE_BODY", QStringList() << "ID", QStringList() << "DISH_ID=" + m_pTable->item(*i, 2)->text(), QStringList() << "ID");
        if (q.next()) {
            not_deleted.push_back(*i);
            m_db.close();
            continue;
        }
        m_db.close();

        QMap<QString, QVariant> w;
        w.insert("DISH_ID", m_pTable->item(*i, 2)->text().toInt());
        del("RECIPES", w);
        w.clear();
        w.insert("ID", m_pTable->item(*i, 2)->text().toInt());
        del("DISHES",  w);
        m_pTable->removeRow(*i);
    }

    if (not_deleted.count()) {
        QString msg = "<html><body><p>%1</p><p><table>";
        for (QList<int>::const_iterator i = not_deleted.begin(); i != not_deleted.end(); i++) {
            msg += QString("<tr><td>%1</td><td>%2</td></tr>")
                    .arg(m_pTable->item(*i, 2)->text())
                    .arg(m_pTable->item(*i, 5)->text());
        }
        msg += "</table></p></body></html>";
        QMessageBox::critical(this, tr("Error"), msg.arg(tr("The next item could not be deleted, because have a dependencies")));
    }
}
