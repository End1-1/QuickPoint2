#ifndef PRINT_H
#define PRINT_H

#include <QtGui>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>

class PaintCommand
{
private:
    const float inch_per_mm;
    float scale;
    QRect printable_area;

protected:
    QPrinter *m_pprinter;
    QPainter *m_ppainter;

protected:
    QPoint &scalePoint(QPoint *point);

public:
    PaintCommand();
    void prepare(QPrinter *printer, QPainter *painter);
    virtual void draw() = 0;
};

class PCNewPage : public PaintCommand
{
public:
    PCNewPage();
    void draw();
};

class PCLine : public PaintCommand
{
private:
    QPoint m_p1;
    QPoint m_p2;
    QPen m_pen;

public:
    PCLine(QPen pen, QPoint p1, QPoint p2);
    void draw();
};

class PCText : public PaintCommand
{
private:
    QFont m_font;
    QPoint m_point;
    QString m_text;

public:
    PCText(QFont font, QPoint p, QString text);
    void draw();
};

class Print
{
private:
    QPrinter *m_pprinter;
    QPainter *m_ppainter;
    QPrintDialog *m_pdlg;

private:
    void draw(QVector<PaintCommand *> &commands);

public:
    Print(QVector<PaintCommand *> &commands);
    Print(QVector<PaintCommand *> &commands, QString printerName);
    ~Print();

};

#endif // PRINT_H
