#include "qnumpadwidget.h"

QNumpadWidget::QNumpadWidget(QWidget *parent) :
    QWidget(parent)
{
    QGridLayout *plButtons = new QGridLayout();
    plButtons->setMargin(0);
    m_pInput = new QLineEdit(this);
    m_pInput->setEchoMode(QLineEdit::Password);
    QPushButton *pbtn_0 = qpushbutton("0"),
            *pbtn_1 = qpushbutton("1"),
            *pbtn_2 = qpushbutton("2"),
            *pbtn_3 = qpushbutton("3"),
            *pbtn_4 = qpushbutton("4"),
            *pbtn_5 = qpushbutton("5"),
            *pbtn_6 = qpushbutton("6"),
            *pbtn_7 = qpushbutton("7"),
            *pbtn_8 = qpushbutton("8"),
            *pbtn_9 = qpushbutton("9"),
            *pbtn_D = qpushbutton("."),
            *pbtn_Ok = qpushbutton(tr("OK")),
            *pbtn_Cancel = qpushbutton("X");

    pbtn_Ok->setMaximumWidth(pbtn_Ok->maximumWidth() * 5);
    int i = 0, j = 0;
    plButtons->addWidget(m_pInput, i, j, 1, 4);
    plButtons->addWidget(pbtn_1, ++i, j = 0);
    plButtons->addWidget(pbtn_2, i, ++j);
    plButtons->addWidget(pbtn_3, i, ++j);
    plButtons->addWidget(pbtn_4, i, ++j);
    plButtons->addWidget(pbtn_5, ++i, j = 0);
    plButtons->addWidget(pbtn_6, i, ++j);
    plButtons->addWidget(pbtn_7, i, ++j);
    plButtons->addWidget(pbtn_8, i, ++j);
    plButtons->addWidget(pbtn_9, ++i, j = 0);
    plButtons->addWidget(pbtn_0, i, ++j);
    plButtons->addWidget(pbtn_D, i, ++j);
    plButtons->addWidget(pbtn_Cancel, i, ++j);
    plButtons->addWidget(pbtn_Ok, ++i, j = 0, 1, 4);

    setMaximumSize(QSize(64 * 4, 64 * 4));
    setLayout(plButtons);

    m_pInput->setFocus();
    setTabOrder(m_pInput, this);

    connect(m_pInput, SIGNAL(returnPressed()), SLOT(onOkKeyPress()));
    connect(pbtn_0, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_1, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_2, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_3, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_4, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_5, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_6, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_7, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_8, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_9, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_D, SIGNAL(clicked()), SLOT(onNumKeyPress()));
    connect(pbtn_Ok, SIGNAL(clicked()), SLOT(onOkKeyPress()));
    connect(pbtn_Cancel, SIGNAL(clicked()), SLOT(onClear()));

    show();
}

void QNumpadWidget::setDefault()
{
    m_pInput->clear();
    m_pInput->setFocus();
}

void QNumpadWidget::onNumKeyPress()
{
    QString key = ((QPushButton *)sender())->text();
    QString curr_text = m_pInput->text();

    if (key == ".")
        if (curr_text.contains(key))
            return;

    m_pInput->setText(curr_text + key);
    m_pInput->setFocus();
    emit onNumKey();
}

void QNumpadWidget::onClear()
{
    setDefault();
}


void QNumpadWidget::onOkKeyPress()
{
    emit Finish(m_pInput->text());
    m_pInput->clear();
}

QPushButton *QNumpadWidget::qpushbutton(const QString &text)
{
    QPushButton *pbtn = new QPushButton(text, this);
    pbtn->setMinimumSize(MINSIZE);
    pbtn->setMaximumSize(MINSIZE);
    return pbtn;
}

void QNumpadWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Escape:
    case Qt::Key_Return:
        return;
    default:
        QWidget::keyPressEvent(e);
    }
}
