#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QMainWindow>
#include "graphicsview.h"
#include "neuron.h"
#include "weight.h"
#include "fonctions.h"
#include <thread>

typedef struct NET_STRUCT{
    unsigned int nbLayers;
    unsigned int *layers;
} netStruct;

typedef struct T_DATA{
    std::vector<double> inp;
    std::vector<double> out;
} TrainingData;


class Network : public QObject
{
    Q_OBJECT
public:
    Network(netStruct structure, int width, int height, QGraphicsScene *scene);

    std::vector<std::vector<Neuron*>> neuronsGraph;
    std::vector<std::vector<std::vector<Weight*>>> weightsGraph;
    netStruct structure;

    void randomize();
    void propagate();
    void propagate(std::vector<double> inp);
    void propagate(TrainingData data);
    void propagate(unsigned int dataIndex);

    void train();

    void set_input(std::vector<double> inp);
    std::vector<double> get_output(double layer = -1);

    std::vector<TrainingData> getData() const;
    void setData(std::vector<TrainingData> newData);

    void stopThread();

    double get_advance();

    inline void setPlot(GraphicsView* plt){ this->plot = plt; }
public slots:
    void prevData();
    void nextData();

signals:
    void propagated();

private:
    std::vector<double> error_calculation(TrainingData data);
    void regregate(std::vector<double> error);
    bool theardRun = true;
    QGraphicsScene *scene = nullptr;
    std::vector<TrainingData> data;
    std::vector<std::vector<std::vector<double>>> get_weights_matrix();

    double startError=-1;
    double currentError;
    double endError = 25;

    unsigned int currentData = 0;

    GraphicsView *plot = nullptr;
};

#endif // NETWORK_H
