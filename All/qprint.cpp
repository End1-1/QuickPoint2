#include "qprint.h"
#include <QtPrintSupport/qprinter.h>

QPrint::QPrint(const QString &printerName)
{
    m_pprinter = new QPrinter();
    m_pprinter->setPrinterName(printerName);
    m_ppainter = new QPainter(m_pprinter);

}

QPrint::~QPrint()
{
    for (QVector<QPrintCommand *>::iterator i = m_printCommands.begin(); i != m_printCommands.end(); i++)
        delete *i;

    delete m_ppainter;
    delete m_pprinter;
}

void QPrint::addCommand(QPrintCommand *printCommand)
{
    printCommand->setPrinter(m_pprinter, m_ppainter);
    m_printCommands.push_back(printCommand);
}

int QPrint::rscale(const int &value)
{
    return value / 0.041 / m_pprinter->resolution();
}

int QPrint::incTop()
{
    return rscale(QFontMetrics(m_font).height());
}

QPCSetFont::QPCSetFont(const QFont &font) :
    m_font(font)
{

}

void QPCSetFont::command()
{
    m_ppainter->setFont(m_font);
}


QPrintCommand::QPrintCommand() :
    inch_per_mm(0.041)
{
}

QPrintCommand::~QPrintCommand()
{

}

QPoint QPrintCommand::scale(const QPoint &point)
{
    QPoint p;
    p.setX(point.x() * inch_per_mm * m_pprinter->resolution());
    p.setY(point.y() * inch_per_mm * m_pprinter->resolution());
    return p;
}

QRect QPrintCommand::scale(const QRect &rect)
{
    QRect r;
    r.setLeft(rect.left() * inch_per_mm * m_pprinter->resolution());
    r.setTop(rect.top() * inch_per_mm * m_pprinter->resolution());
    r.setRight(rect.right() * inch_per_mm * m_pprinter->resolution());
    r.setBottom(rect.bottom() * inch_per_mm * m_pprinter->resolution());
    return r;
}

void QPrintCommand::setPrinter(QPrinter *printer, QPainter *painter)
{
    m_pprinter = printer;
    m_ppainter = painter;
}


void QPrint::beginPrint()
{
    for (QVector<QPrintCommand *>::const_iterator i = m_printCommands.begin(); i != m_printCommands.end(); i++)
        (*i)->command();
}

void QPrint::cmdSetFont(QFont font)
{
    m_font = font;
    addCommand(new QPCSetFont(m_font));
}

void QPrint::cmdNewPage()
{
    addCommand(new QPCNewPage());
}

void QPrint::cmdTextRect(const QString &text, const QRect &rect, const QTextOption &to)
{
    addCommand(new QPCTextRect(text, rect, to));
}


void QPCNewPage::command()
{
    m_pprinter->newPage();
}


QPCBitmap::QPCBitmap(const QRect &rect, const QString &filename) :
    m_rect(rect), m_filename(filename)
{

}

void QPCBitmap::command()
{
    m_ppainter->drawPixmap(scale(m_rect), QPixmap(m_filename));
}


QPCText::QPCText(const QString &text, const QPoint &point) :
    m_text(text), m_point(point)
{
}

void QPCText::command()
{
    m_ppainter->drawText(scale(m_point), m_text);
}


QPCSetPen::QPCSetPen(const QPen &pen) :
    m_pen(pen)
{
}

void QPCSetPen::command()
{
    m_ppainter->setPen(m_pen);
}


QPCLine::QPCLine(const QPoint &start, const QPoint &end) :
    m_start(start), m_end(end)
{
}

void QPCLine::command()
{
    m_ppainter->drawLine(scale(m_start), scale(m_end));
}


QPCTextRect::QPCTextRect(const QString &text, const QRect &rect, const QTextOption &to) :
    m_text(text), m_rect(rect), m_to(to)
{
}

void QPCTextRect::command()
{
    m_ppainter->drawText(scale(m_rect), m_text, m_to);
}
