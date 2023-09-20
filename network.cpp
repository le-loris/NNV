#include "network.h"
#include <QFile>

Network::Network(netStruct structure, int width, int height, QGraphicsScene *scene) : QObject()
{
    srand(time(NULL));
    this->scene = scene;
    this->structure = structure;
    int shiftY, shiftX = width/(structure.nbLayers);

    for(unsigned int layerNum = 0; layerNum < structure.nbLayers; layerNum++){
        shiftY = height /  (structure.layers[layerNum] + 1);
        std::vector<Neuron*> layer;
        for(unsigned int neuronNum = 0; neuronNum < structure.layers[layerNum]; neuronNum++){
            auto neuron = new Neuron(QPoint((layerNum)*shiftX, (neuronNum+1)*shiftY), range_rand(1));
            this->scene->addItem(neuron);
            layer.push_back(neuron);
        }
        this->neuronsGraph.push_back(layer);

        if(layerNum > 0){
            std::vector<std::vector<Weight*>> layerW;
            for(unsigned int i = 0; i < structure.layers[layerNum-1]; i++){
                auto parent = this->neuronsGraph.at(layerNum-1).at(i);
                auto children = this->neuronsGraph.at(layerNum);
                std::vector<Weight*> neuronW;
                for(unsigned int j = 0; j < structure.layers[layerNum]; j++){
                    auto weight = new Weight(parent->getOutput(), children.at(j)->getInput(), range_rand(1));
                    this->scene->addItem(weight);
                    neuronW.push_back(weight);
                }
                layerW.push_back(neuronW);
            }
            this->weightsGraph.push_back(layerW);
        }
    }
    scene->update();
}

void Network::propagate()
{
    double cumul = 0;
    for(unsigned int l = 1; l < this->structure.nbLayers; l++){
        for(unsigned int n = 0; n < this->neuronsGraph.at(l).size(); n++){
            cumul = 0;
            for(unsigned int p = 0; p < this->neuronsGraph.at(l-1).size(); p++){
                cumul  += this->neuronsGraph.at(l-1).at(p)->value * this->weightsGraph.at(l-1).at(p).at(n)->value ;
            }
            activation(cumul);
            this->neuronsGraph.at(l).at(n)->value = cumul;
        }
    }

//    cumul = 0;
//    for(unsigned int n = 0; n < this->neuronsGraph.back().size(); n++)
//        cumul += this->neuronsGraph.back().at(n)->value;

//    for(unsigned int n = 0; n < this->neuronsGraph.back().size(); n++)
//        this->neuronsGraph.back().at(n)->value /= cumul;

}

void Network::propagate(std::vector<double> inp){
    if(inp.size() != this->structure.layers[0]){
        qWarning() << "Input data has invalid format";
        return;
    }

    set_input(inp);
    propagate();
}

void Network::propagate(TrainingData data){
    propagate(data.inp);
}

void Network::propagate(unsigned int dataIndex){
    propagate(this->data.at(dataIndex));
}

void Network::randomize(){
    for(auto neuron : this->neuronsGraph.at(0)){
        neuron->value = range_rand();
    }
}

void Network::set_input(std::vector<double> inp){
    if(inp.size() != this->structure.layers[0]){
        qWarning() << "Input data has invalid format";
        return;
    }

    for(unsigned int i = 0; i < inp.size(); i++){
        this->neuronsGraph.at(0).at(i)->value = inp.at(i);
    }
}

std::vector<double> Network::get_output(double layer){
    if(layer < 0)
        layer = this->structure.nbLayers-1;

    std::vector<double> result;

    for(unsigned int i = 0; i < this->neuronsGraph.at(layer).size(); i++){
        result.push_back(this->neuronsGraph.at(layer).at(i)->value);
    }
    return result;
}

std::vector<double> Network::error_calculation(TrainingData data){
    this->set_input(data.inp);
    this->propagate();

    auto result = this->get_output();

    if(result.size() != data.out.size())
        qWarning() << "Training data has invalid format";

    std::vector<double> error;
    double temp;
    double temp2 = 0;
    for(unsigned int i = 0; i < result.size(); i++){
        temp = data.out.at(i) - result.at(i);
        error.push_back(temp);
        temp2 += temp*temp;

    }

    error.push_back(10000*temp2);

    return error;
}

std::vector<std::vector<std::vector<double>>> Network::get_weights_matrix(){
    std::vector<std::vector<std::vector<double>>> matrix;
    std::vector<double> buf1;
    std::vector<std::vector<double>> buf2;

    for(auto layer : this->weightsGraph){
        for(auto neuron : layer){
            for(auto connexion : neuron){
                buf1.push_back(connexion->value);
            }
            buf2.push_back(buf1); buf1.clear();
        }
        matrix.push_back(buf2); buf2.clear();
    }

    return matrix;
}

void Network::regregate(std::vector<double> error){
    std::vector<std::vector<std::vector<double>>> matrix = get_weights_matrix();
    std::vector<std::vector<double>> error_spread;
    std::vector<double> buf;

    error_spread.push_back(error);

    for(unsigned int layer = this->structure.nbLayers - 0; layer > 1; layer--){
        {
            buf = multiply(matrix.at(layer-2), error_spread.back());
        }
        error_spread.push_back(buf); buf.clear();
    }
    std::vector<std::vector<double>> deltas;

    for(unsigned int i = this->structure.nbLayers; i > 1; i--){
        std::vector<double> layer_error = error_spread.at(this->structure.nbLayers-i);
        std::vector<double> layer_value = get_output(i-1);
        std::vector<double> precd_value = get_output(i-2);
        std::vector<double> grad_vector = child_product(layer_error, layer_value);
        deltas = multiply(grad_vector, precd_value, 0.02);


        for(unsigned int n = 0; n < this->weightsGraph.at(i-2).size(); n++){
            for(unsigned int w = 0; w < this->weightsGraph.at(i-2).at(n).size(); w++){
                //qDebug() << deltas.at(w).at(n);
                this->weightsGraph.at(i-2).at(n).at(w)->value += deltas.at(w).at(n);
            }
        }
        //propagate();
    }

    for(auto layer : this->weightsGraph)
        for(auto neuron : layer)
            for(auto weight : neuron)
                weight->value = MIN(MAX(weight->value,0),1);
}

void Network::stopThread(){
    this->theardRun = false;
}

std::vector<TrainingData> Network::getData() const
{
    return data;
}

void Network::setData(std::vector<TrainingData> newData)
{
    data = newData;
}

void Network::train(){
    std::vector<double> *errors = new std::vector<double>[this->data.size()];
    double avgError;

    while(this->theardRun){
        avgError = 0;
        this->currentError = 0;
        for(currentData = 0; currentData < this->data.size(); currentData++){
            auto tData = this->data.at(currentData);
            std::vector<double> err = error_calculation(tData);
            double genErr = err.back(); err.pop_back();
            avgError += genErr;
            currentError = MAX(currentError,genErr);

            //qDebug() << d << ":" << genErr/10.f << "%";
            errors[currentData].push_back(genErr);
            regregate(err);

        }
        avgError /= this->data.size();

        if(((avgError < 40) && (this->currentError < this->endError)) || (errors[0].size() >= 50000))
            this->theardRun = false;

        if(startError < 0)
            startError = currentError;

        //if(errors[0].size() % 100 == 0)
            emit propagated();
    }

    qDebug() << avgError << errors[0].size();
    QFile f("../NNV/log.csv");
    f.open(QFile::WriteOnly);
    for(unsigned int i = 0; i < errors[0].size(); i++){
        QString s = QString("\t") + QString::number(i);
        f.write(s.toStdString().data());
    }
    for(unsigned int i = 0; i < this->data.size(); i++){
        qDebug() << i << ":" << QString::asprintf("%.2f%%", errors[i].back());
        f.write(QString::number(i).toStdString().data());
        for(auto val : errors[i]){
            QString s = QString("\t") + QString::asprintf("%.2f", val);
            f.write(s.toStdString().data());
        }
        f.write("\n");
    }
    f.close();
}

double Network::get_advance(){
    if(this->currentError<this->endError)
        return 100;
    else
        return 100*(1 - ((1 - this->currentError/this->endError)/(1 - this->startError/this->endError)));
}

void Network::nextData(){
    this->currentData = (this->currentData + 1) % this->data.size();
    this->propagate(this->currentData);
    emit propagated();
}
void Network::prevData(){
    this->currentData += this->data.size();
    this->currentData = (this->currentData - 1) % this->data.size();
    this->propagate(this->currentData);
    emit propagated();
}
