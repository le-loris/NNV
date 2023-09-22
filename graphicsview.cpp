#include "graphicsview.h"
#include <QGraphicsPixmapItem>

GraphicsView::GraphicsView(QWidget* widget, graphic_type type) : QGraphicsView(widget)
{
    this->type = type;
    this->scene = new QGraphicsScene(this);
    this->setScene(this->scene);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap pix = QPixmap(this->size());
    pix.fill(Qt::transparent);
    pixItem = this->scene->addPixmap(pix);

}

void GraphicsView::update()
{
//    if(this->type == hRule)
//        pixItem->setPixmap(drawHRules());
//    if(this->type == vRule)
//        pixItem->setPixmap(drawVRules());


    if(this->type == PLOT){
        this->origin = QPoint(30, this->height()-25);
        this->plotSize = QSize((this->width()-25)*0.95, (this->height()-25)*0.90);
        auto pix = QPixmap(this->size());
        pix.fill(Qt::transparent);
        drawPlot(pix);
        drawHRules(pix);
        drawVRules(pix);
        pixItem->setPixmap(pix);
    }

    this->scene->update();
    QGraphicsView::update();
}

void GraphicsView::drawHRules(QPixmap &pix){
    //QPixmap pix(this->size());
    QPainter p(&pix);
    QPen pen = QPen(Qt::black, 1);
    p.setPen(pen);

    float inc = this->plotSize.width() * pow(10, int(log10(2*scale.width())-1)) / scale.width();

    float i = this->origin.x()+inc;
    QString text;
    p.drawLine(0, this->origin.y(), this->width(), this->origin.y());
    while(i <= 1+this->origin.x()+this->plotSize.width()){
        text = QString::asprintf("%.0f", 100*scale.width() * (i-this->origin.x())/this->plotSize.width());
        p.drawText(i, this->origin.y()+20, text);
        p.drawLine(i, this->origin.y(), i, this->origin.y()+5);
        i+= inc; //this->plotSize.width() / 20.f;
    }
    p.end();
    //return pix;
}

void GraphicsView::drawVRules(QPixmap &pix){
    //QPixmap pix(this->size());
    QPainter p(&pix);
    QPen pen = QPen(Qt::black, 1);
    p.setPen(pen);

    float i = this->origin.y() - this->plotSize.height() / 10.f;
    QString text;
    p.drawLine(this->origin.x(), 0, this->origin.x(), this->height());
    while(i >= this->origin.y() - this->plotSize.height()){
        text = QString::asprintf("%.0f%%", 0.01*scale.height() * (this->origin.y() - i)/this->plotSize.height());
        p.drawText(0, i+5, text);
        p.drawLine(25, i, 30, i);
        i-= this->plotSize.height() / 10.f;
    }
    p.end();
    //return pix;
}

QPointF GraphicsView::toScale(QPointF p){
    return QPoint((origin.x() + this->plotSize.width()*float(p.x()) / scale.width()) , this->origin.y() - this->plotSize.height()*float(p.y()) / scale.height());
}

void GraphicsView::drawPlot(QPixmap &pix){
    if(this->points.size() <= 1)
        return;

    QPainter p(&pix);
    QPen pen = QPen(Qt::black, 1);
    QBrush brush = QBrush(Qt::black);
    p.setPen(pen);p.setBrush(brush);

    QPointF last = this->points.at(0);

    this->scale.setHeight(1000*(1+int(last.y()/1000)));
    this->scale.setWidth(10*(1+int(this->points.back().x()/10)));

    for(auto point : points){
        p.drawLine(toScale(last), toScale(point));
        last = point;
    }

    QPointF ref = toScale(last);

    p.drawEllipse(ref, 3, 3);
    p.drawText(QPointF(ref.x()+3, ref.y()-3), QString::asprintf("%.1f‰", last.y()/10));
    p.end();
}
