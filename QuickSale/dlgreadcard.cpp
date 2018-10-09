#include "dlgreadcard.h"
#include "ui_dlgreadcard.h"

DlgReadCard::DlgReadCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgReadCard)
{
    ui->setupUi(this);
}

DlgReadCard::~DlgReadCard()
{
    delete ui;
}

bool DlgReadCard::cardCode(QString &code)
{
    bool result = false;
    DlgReadCard *d = new DlgReadCard(0);
    if (d->exec() == QDialog::Accepted) {
        code = d->ui->leCode->text().replace("?", "").replace(";", "");
        result = code.length() > 0;
    }
    delete d;
    return result;
}

void DlgReadCard::on_btnCancel_clicked()
{
    reject();
}

void DlgReadCard::on_btnOK_clicked()
{
    accept();
}
