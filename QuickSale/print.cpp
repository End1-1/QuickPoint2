#include "print.h"

Print::Print(QVector<PaintCommand *> &commands)
{
    m_pprinter = new QPrinter();
    m_pdlg = new QPrintDialog(m_pprinter);
    if (m_pdlg->exec() == QPrintDialog::Accepted)
    {
        m_ppainter = new QPainter(m_pprinter);
        draw(commands);
        delete m_ppainter;
    }
    delete m_pdlg;
    delete m_pprinter;
}

Print::Print(QVector<PaintCommand *> &commands, QString printerName)
{
    m_pprinter = new QPrinter();
    m_pprinter->setPrinterName(printerName);
    m_ppainter = new QPainter(m_pprinter);
    draw(commands);
    delete m_ppainter;
    delete m_pprinter;
}

Print::~Print()
{
}

void Print::draw(QVector<PaintCommand *> &commands)
{
    for (QVector<PaintCommand *>::iterator i = commands.begin(); i != commands.end(); i++)
    {
        (*i)->prepare(m_pprinter, m_ppainter);
        (*i)->draw();
        delete *i;
    }
}

PCLine::PCLine(QPen pen, QPoint p1, QPoint p2)
    : m_p1(p1), m_p2(p2), m_pen(pen)
{
}

void PCLine::draw()
{
    m_ppainter->setPen(m_pen);
    m_ppainter->drawLine(scalePoint(&m_p1), scalePoint(&m_p2));
}


QPoint &PaintCommand::scalePoint(QPoint *point)
{
    point->setX((point->x() * scale) + printable_area.x());
    point->setY((point->y() * scale) + printable_area.y());
    return *point;
}

PaintCommand::PaintCommand() : inch_per_mm(0.041)
{
}

void PaintCommand::prepare(QPrinter *printer, QPainter *painter)
{
    m_pprinter = printer;
    m_ppainter = painter;
    printable_area = m_ppainter->viewport();
    scale = inch_per_mm * m_pprinter->resolution();
}

PCNewPage::PCNewPage()
{
}

void PCNewPage::draw()
{
    m_pprinter->newPage();
}


PCText::PCText(QFont font, QPoint p, QString text) :
    m_font(font), m_point(p), m_text(text)
{
}

void PCText::draw()
{
    m_ppainter->setFont(m_font);
    m_ppainter->drawText(scalePoint(&m_point), m_text);
}
