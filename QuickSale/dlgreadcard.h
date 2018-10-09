#ifndef DLGREADCARD_H
#define DLGREADCARD_H

#include <QDialog>

namespace Ui {
class DlgReadCard;
}

class DlgReadCard : public QDialog
{
    Q_OBJECT

public:
    explicit DlgReadCard(QWidget *parent = 0);
    ~DlgReadCard();
    static bool cardCode(QString &code);
private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::DlgReadCard *ui;
};

#endif // DLGREADCARD_H
