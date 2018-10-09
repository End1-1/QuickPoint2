#include "ecardappend.h"
#include "ui_ecardappend.h"
#include <QSqlQuery>

ECardAppend::ECardAppend(QWidget *parent) :
    EDialog(parent),
    ui(new Ui::ECardAppend)
{
    ui->setupUi(this);
}

ECardAppend::~ECardAppend()
{
    delete ui;
}

void ECardAppend::load(int id)
{
    M::db.db.open();
    QSqlQuery q(M::db.db);
    q.prepare("select cardnum, cardcode, customer, amount, val from cards_append where id=:id");
    q.bindValue(":id", id);
    q.exec();
    q.next();
    ui->leId->setText(QString::number(id));
    ui->leNumber->setText(q.value(0).toString());
    ui->leCode->setText(q.value(1).toString());
    ui->leCustomer->setText(q.value(2).toString());
    ui->leAmount->setText(q.value(3).toString());
    ui->leVal->setText(q.value(4).toString());
}

void ECardAppend::on_btnCancel_clicked()
{
    reject();
}

void ECardAppend::on_btnSave_clicked()
{

    if (ui->leId->text().isEmpty()) {
        int id = M::db.GetID("GEN_CARDS_APPEND_ID");
        M::db.db.open();
        QSqlQuery qq(M::db.db);
        ui->leId->setText(QString::number(id));
        qq.prepare("insert into cards_append(id) values (:id)");
        qq.bindValue(":id", ui->leId->text().toInt());
        qq.exec();
    }
    QSqlQuery q(M::db.db);
    q.prepare("update cards_append set cardnum=:cardnum, cardcode=:cardcode, customer=:customer, val=:val where id=:id");
    q.bindValue(":id", ui->leId->text().toInt());
    q.bindValue(":cardnum", ui->leNumber->text());
    q.bindValue(":cardcode", ui->leCode->text());
    q.bindValue(":customer", ui->leCustomer->text());
    q.bindValue(":val", ui->leVal->text().toDouble());
    q.exec();
    M::db.commit();
    done(ui->leId->text().toInt());
}
