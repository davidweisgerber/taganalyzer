#include <QPainter>
#include "linechart.h"

LineChart::LineChart(const QRectF &boundingRect) :
    m_boundingRect(boundingRect),
    m_freeBottom(30),
    m_freeLeft(0)
{

}

LineChart::~LineChart()
{

}

QRectF LineChart::boundingRect() const
{
    return m_boundingRect;
}

void LineChart::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->fillRect(m_boundingRect, Qt::white);

    if (m_values.isEmpty())
    {
        return;
    }

    calculateLimits();
    painter->setPen(Qt::black);
    painter->drawRect(m_freeLeft, 0, m_boundingRect.width() - m_freeLeft, m_boundingRect.height() - m_freeBottom);
    drawTimeRaster(painter);
    drawValueRaster(painter);
    drawReferenceGraph(painter);
    drawValueGraphs(painter);
    drawResult(painter);
}

void LineChart::setReference(const QMap<QDateTime, int> &reference)
{
    m_reference = reference;
    update();
}

void LineChart::setResult(const QMap<QDateTime, int> &result)
{
    m_result = result;
    update();
}

void LineChart::addValue(const QMap<QDateTime, int> &value)
{
    m_values.append(value);
    update();
}

void LineChart::clear()
{
    m_values.clear();
    update();
}

void LineChart::calculateLimits()
{
    m_from = QDateTime();
    m_to = QDateTime();

    if (m_values.size() > 0)
    {
        if (m_values[0].size() > 0)
        {
            m_from = m_values[0].keys()[0];
            m_to = m_from;
        }
    }

    for (int i=0; i < m_values.size(); i++)
    {
        QList<QDateTime> keys = m_values[i].keys();
        m_topLimits.push_back(0);
        m_bottomLimits.push_back(0);
        m_spans.push_back(0);
        m_dpV.push_back(0);
        if (keys.size() > 0)
        {
            m_topLimits[i] = m_values[i][keys[0]];
            m_bottomLimits[i] = m_topLimits[i];
        }

        foreach (QDateTime dateTime, keys)
        {
            int value = m_values[i][dateTime];
            m_from = dateTime < m_from ? dateTime : m_from;
            m_to = dateTime > m_to ? dateTime : m_to;
            m_topLimits[i] = qMax(m_topLimits[i], value);
            m_bottomLimits[i] = qMin(m_bottomLimits[i], value);
        }

        m_spans[i] = m_topLimits[i] - m_bottomLimits[i];
        m_dpV[i] = static_cast<double>(m_boundingRect.height() - m_freeBottom) / m_spans[i];
    }

    m_from = m_from.addSecs(60 * 60 * -1);
    m_to = m_to.addSecs(60 * 60);
    m_freeLeft = 100; //!TODO: Calculate text width on the left
    m_span = m_from.secsTo(m_to);
    m_dpS = (m_boundingRect.width() - m_freeLeft) / static_cast<double>(m_span);
    m_dpMgdl = static_cast<double>(m_boundingRect.height() - m_freeBottom) / 400;
}

void LineChart::drawTimeRaster(QPainter *painter)
{
    painter->setFont(QFont("Arial", 9));

    QDateTime firstLine = m_from;
    firstLine.setTime(QTime(m_from.time().hour() + 1, 0));
    for (QDateTime current=firstLine; current < m_to; current = current.addSecs(60*60))
    {
        double distance = m_from.secsTo(current) * m_dpS;
        distance += m_freeLeft;

        painter->setPen(QColor(200, 200, 200));
        painter->drawLine(distance, 0, distance, m_boundingRect.height() - m_freeBottom);
        painter->setPen(Qt::black);
        painter->drawText(distance - 100, m_boundingRect.height() - m_freeBottom, 200, 30, Qt::AlignHCenter | Qt::AlignVCenter, QString("%1\n%2").
                          arg(current.time().toString("HH:mm")).arg(current.date().toString("yyyy-MM-dd")));
    }
}

void LineChart::drawValueRaster(QPainter *painter)
{
    int value = 100;
    int y = (m_boundingRect.height() - m_freeBottom) - (value * m_dpMgdl);
    while (y > 0)
    {
        painter->setPen(QColor(200, 200, 200));
        painter->drawLine(m_freeLeft, y, m_boundingRect.width(), y);
        painter->setPen(Qt::black);
        painter->drawText(0, y - 15, m_freeLeft, 30, Qt::AlignHCenter | Qt::AlignVCenter, QString::number(value));
        value += 100;
        y = (m_boundingRect.height() - m_freeBottom) - (value * m_dpMgdl);
    }
}

void LineChart::drawReferenceGraph(QPainter *painter)
{
    QList<QDateTime> keys = m_reference.keys();
    qSort(keys);

    QPolygon polygon;
    foreach (QDateTime dateTime, keys)
    {
        if (dateTime < m_from)
        {
            continue;
        }
        else if (dateTime > m_to)
        {
            break;
        }

        double distance = m_from.secsTo(dateTime) * m_dpS;
        distance += m_freeLeft;
        double value = m_reference[dateTime] * m_dpMgdl;
        polygon << QPoint(distance, m_boundingRect.height() - m_freeBottom - value);
    }

    painter->setPen(Qt::black);
    painter->drawPolyline(polygon);
}

void LineChart::drawValueGraphs(QPainter *painter)
{
    for (int i=0; i < m_values.size(); i++)
    {
        QList<QDateTime> keys = m_values[i].keys();
        qSort(keys);

        QPolygon polygon;
        foreach (QDateTime dateTime, keys)
        {
            double distance = m_from.secsTo(dateTime) * m_dpS;
            distance += m_freeLeft;
            double value = (m_values[i][dateTime] - m_bottomLimits[i]) * m_dpV[i];
            polygon << QPoint(distance, m_boundingRect.height() - m_freeBottom - value);
        }

        switch (i)
        {
        case 0:
            painter->setPen(Qt::blue);
            break;
        case 1:
            painter->setPen(Qt::red);
            break;
        default:
            painter->setPen(Qt::green);
            break;
        }

        painter->drawPolyline(polygon);
    }
}

void LineChart::drawResult(QPainter *painter)
{
    QList<QDateTime> keys = m_result.keys();
    qSort(keys);

    QPolygon polygon;
    foreach (QDateTime dateTime, keys)
    {
        double distance = m_from.secsTo(dateTime) * m_dpS;
        distance += m_freeLeft;
        double value = m_result[dateTime] * m_dpMgdl;
        polygon << QPoint(distance, m_boundingRect.height() - m_freeBottom - value);
    }

    painter->setPen(Qt::green);
    painter->drawPolyline(polygon);
}

