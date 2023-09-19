#include "weight.h"
#include "fonctions.h"
#include <QPainter>
Weight::Weight(QPointF begin, QPointF end, double value)
{
    this->value = value;
    this->setLine(begin.x(), begin.y(), end.x(), end.y());
}

void Weight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if((this->value > 1) || (this->value < 0)){
        //qDebug() << "VALUE ERROR :" << this->value;
        this->value = MAX(MIN(this->value, 1), 0);
    }
    this->color = QColor(int(255*(1-this->value)), int(255*this->value), 0);
    this->setPen(QPen(QBrush(this->color), 3));

    QGraphicsLineItem::paint(painter, option, widget);

}
