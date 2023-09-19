#ifndef WEIGHT_H
#define WEIGHT_H

#include <QGraphicsLineItem>
#include <QWidget>

class Weight : public QGraphicsLineItem
{
public:
    Weight(QPointF begin = QPointF(0, 0), QPointF end = QPointF(0, 0), double value = 0);

    double value;
    QColor color;

public slots:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // WEIGHT_H
