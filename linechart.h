#ifndef LINECHART_H
#define LINECHART_H

#include <QGraphicsItem>
#include <QDateTime>
#include <QList>
#include <QMap>

class LineChart : public QGraphicsItem
{
public:
    LineChart(const QRectF &boundingRect);
    ~LineChart();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setReference(const QMap<QDateTime, int> &reference);
    void setResult(const QMap<QDateTime, int> &result);
    void addValue(const QMap<QDateTime, int> &value);
    void clear();

private:
    void calculateLimits();
    void drawTimeRaster(QPainter *painter);
    void drawValueRaster(QPainter *painter);
    void drawReferenceGraph(QPainter *painter);
    void drawValueGraphs(QPainter *painter);
    void drawResult(QPainter *painter);

    QRectF m_boundingRect;
    QMap<QDateTime, int> m_reference;
    QMap<QDateTime, int> m_result;
    QList<QMap<QDateTime, int> > m_values;
    QList<int> m_topLimits;
    QList<int> m_bottomLimits;
    QList<int> m_spans;
    QList<double> m_dpV;

    QDateTime m_from;
    QDateTime m_to;
    int m_span;
    double m_dpS;
    double m_dpMgdl;
    int m_freeBottom;
    int m_freeLeft;
};

#endif // LINECHART_H
