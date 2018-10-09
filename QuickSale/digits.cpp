#include "digits.h"
#include "kbd.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

Digits::Digits(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *vblMain = new QVBoxLayout(this);
    QHBoxLayout *hblButton = new QHBoxLayout(this);
    QWidget *wdtButton = new QWidget(this);

    buffer = new QLineEdit(this);
    kbd = new Kbd(this);
    QPushButton *pbOk = new QPushButton(tr("OK"), this);
    QPushButton *pbCancel = new QPushButton(tr("Cancel"), this);

    connect(kbd, SIGNAL(signalChar(QString)), SLOT(slotChar(QString)));
    connect(pbOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pbCancel, SIGNAL(clicked()), SLOT(reject()));

    hblButton->addStretch(1);
    hblButton->addWidget(pbOk);
    hblButton->addWidget(pbCancel);
    hblButton->addStretch(1);
    wdtButton->setLayout(hblButton);

    vblMain->addWidget(buffer);
    vblMain->addWidget(kbd);
    vblMain->addWidget(wdtButton);
    setLayout(vblMain);
}

bool Digits::value(double &value)
{
    bool result = false;
    Digits *d = new Digits();
    d->buffer->setText(QString::number(value, 'f', 0));
    if (d->exec() == QDialog::Accepted)
        result = true;
    value = d->buffer->text().toDouble();
    delete d;
    return result;
}

void Digits::slotChar(const QString &value)
{
    if (value.toUpper() == "C")
    {
        buffer->clear();
        return;
    }

    if (value == ".")
        if (!buffer->text().contains("."))
        {
            buffer->setText(buffer->text() + value);
            return;
        }

    if (buffer->text() == "0")
    {
        buffer->setText(value);
        return;
    }

    buffer->setText(buffer->text() + value);
}
