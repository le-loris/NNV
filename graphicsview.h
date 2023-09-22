#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QObject>
#include <QWidget>

enum graphic_type{
    DEFAULT = 0,
    PLOT,
    hRule,
    vRule
};

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget* widget = nullptr, graphic_type type = DEFAULT);

    inline QGraphicsScene* getScene() {return this->scene;}

    void drawHRules(QPixmap &pix);
    void drawVRules(QPixmap &pix);

    inline void setType(graphic_type type) {this->type = type;}

    inline void append(QPoint p) { this->points.push_back(p); }

    void drawPlot(QPixmap &pix);

public slots:
    void update();

private:
    QGraphicsScene *scene = nullptr;
    QSize scale = QSize(200, 1000);
    QPointF toScale(QPointF p);

    QGraphicsPixmapItem *pixItem = nullptr;

    graphic_type type = DEFAULT;
    QPoint origin;
    QSize plotSize;

    std::vector<QPointF> points;
};

#endif // GRAPHICSVIEW_H
