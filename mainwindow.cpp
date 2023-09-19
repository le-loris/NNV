#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto args = QCoreApplication::arguments();
    args.pop_front();

    if(args.size() != 0){
        structure.nbLayers = args.size();
        structure.layers = new unsigned int[structure.nbLayers];
        for(unsigned int i = 0; i < structure.nbLayers; i++)
            structure.layers[i] = args.at(i).toInt();
    }

    else{
        QList l = {3, 5, 5, 2};
        structure.nbLayers = l.size();
        structure.layers = new unsigned int[structure.nbLayers];
        for(unsigned int i = 0; i < structure.nbLayers; i++)
            structure.layers[i] = l.at(i);
    }

    this->scene = new QGraphicsScene();
    this->ui->graphicsView->setScene(scene);

    create_data();

//    this->resize(960, 1060);
//    this->move(0,0);
//    this->show();
    this->showMaximized();
    connect(frames, &QTimer::timeout, this, &MainWindow::start);
    frames->setSingleShot(true);
    frames->start(50);
}

void MainWindow::start(){
    this->network = new Network(structure, this->width(), this->height(), this->scene);
    this->network->setData(data);
    connect(this->network, SIGNAL(propagated()), this->scene, SLOT(update()));
    connect(this->ui->next_button, SIGNAL(clicked(bool)), this->network, SLOT(nextData()));
    connect(this->ui->prev_button, SIGNAL(clicked(bool)), this->network, SLOT(prevData()));

    this->network->propagate();
    this->scene->update();

    this->tm = new ThreadManager(this->network);

}

void MainWindow::show(){
    QMainWindow::show();
}

void MainWindow::create_data(){
    std::vector<double> inp = {0.150, 0.700, 0.150};
    std::vector<double> out = {0.800, 0.200};
    data.push_back(TrainingData{inp, out});

    inp = {0.700, 0.150, 0.150};
    out ={0.400, 0.600};
    data.push_back(TrainingData{inp, out});

    inp = {0.150, 0.150, 0.700};
    out ={0.100, 0.900};
    data.push_back(TrainingData{inp, out});

    inp = {0.200, 0.600, 0.200};
    out ={0.700, 0.300};
    data.push_back(TrainingData{inp, out});

    inp = {0.600, 0.300, 0.100};
    out ={0.500, 0.500};
    data.push_back(TrainingData{inp, out});
}

MainWindow::~MainWindow()
{
    delete tm;
    delete ui;
}

