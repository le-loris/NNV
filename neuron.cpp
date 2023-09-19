#include "neuron.h"
#include "fonctions.h"
#include<QPainter>
Neuron::Neuron(QPoint pos, double value)
{
    this->value = value;
    this->setRect(QRect(0, 0, 100, 100));
    this->setPos(pos);

    this->out = QPoint(this->boundingRect().width(),    this->boundingRect().height()/2);
    this->inp = QPoint(0,                               this->boundingRect().height()/2);

}

void Neuron::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

    if((this->value > 1) || (this->value < 0)){
        //qDebug() << "VALUE ERROR :" << this->value;
        this->value = MAX(MIN(this->value, 1), 0);
    }

    this->color = QColor(int(255*(1-this->value)), int(255*this->value), 0);
    this->setBrush(QBrush(this->color));

    QGraphicsEllipseItem::paint(painter, option, widget);

    QFont f; f.setPixelSize(this->boundingRect().height()*0.25);
    painter->setFont(f);
    painter->drawText(this->boundingRect(), Qt::AlignCenter, QString::asprintf("%.1f", this->value*100)+"%");
    painter->setPen(QPen(QBrush(Qt::black), 6));
}

void Neuron::show(){
    QGraphicsEllipseItem::show();
}

QPointF Neuron::getInput(){
    return mapToScene(inp);
}


QPointF Neuron::getOutput(){
    return mapToScene(out);
}
