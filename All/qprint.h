#ifndef QPRINT_H
#define QPRINT_H

#include <QtGui>
#include <QtPrintSupport/QPrintDialog>

class QPrintCommand
{
protected:
    const float inch_per_mm;
    QPrinter *m_pprinter;
    QPainter *m_ppainter;

public:
    QPrintCommand();
    virtual ~QPrintCommand();
    QPoint scale(const QPoint &point);
    QRect scale(const QRect &rect);
    void setPrinter(QPrinter *printer, QPainter *painter);
    virtual void command() = 0;
};

class QPCNewPage : public QPrintCommand
{
protected:
    void command();
};

class QPCSetFont : public QPrintCommand
{
private:
    QFont m_font;

public:
    QPCSetFont(const QFont &font);

protected:
    void command();
};

class QPCSetPen : public QPrintCommand
{
private:
    QPen m_pen;

public:
    QPCSetPen(const QPen &pen);

protected:
    void command();
};

class QPCLine : public QPrintCommand
{
private:
    QPoint m_start;
    QPoint m_end;

public:
    QPCLine(const QPoint &start, const QPoint &end);

protected:
    void command();
};

class QPCText : public QPrintCommand
{
private:
    QString m_text;
    QPoint m_point;

public:
    QPCText(const QString &text, const QPoint &point);

protected:
    void command();

};

class QPCTextRect : public QPrintCommand
{
private:
    QString m_text;
    QRect m_rect;
    QTextOption m_to;

public:
    QPCTextRect(const QString &text, const QRect &rect, const QTextOption &to = QTextOption());

protected:
    void command();
};

class QPCBitmap : public QPrintCommand
{
private:
    QRect m_rect;
    QString m_filename;

public:
    QPCBitmap(const QRect &rect, const QString &filename);
    void command();
};

class QPrint
{
private:
    QPainter *m_ppainter;
    QPrinter *m_pprinter;
    QVector<QPrintCommand *> m_printCommands;

public:
    QPrint(const QString &printerName = "");
    ~QPrint();
    void addCommand(QPrintCommand *printCommand);
    int rscale(const int &value);
    int incTop();
    void beginPrint();

    QFont m_font;
    void cmdSetFont(QFont font);
    void cmdNewPage();
    void cmdTextRect(const QString &text, const QRect &rect, const QTextOption &to);
};

#endif // QPRINT_H
