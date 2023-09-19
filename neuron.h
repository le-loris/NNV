#ifndef NEURON_H
#define NEURON_H

#include <QGraphicsEllipseItem>
#include <QWidget>

class Neuron : public QGraphicsEllipseItem
{
public:
    Neuron(QPoint pos = QPoint(0, 0), double value = 0);

    double value;
    QColor color;

    QPoint inp = QPoint();
    QPoint out = QPoint();
    QPointF getInput();
    QPointF getOutput();
public slots:
    void show();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // NEURON_H
