#ifndef ECARDAPPEND_H
#define ECARDAPPEND_H

#include "edialog.h"

namespace Ui {
class ECardAppend;
}

class ECardAppend : public EDialog
{
    Q_OBJECT

public:
    explicit ECardAppend(QWidget *parent = 0);
    ~ECardAppend();
    void load(int id);
private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::ECardAppend *ui;
};

#endif // ECARDAPPEND_H
