#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QTimer>
#include "network.h"
#include "threadmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QGraphicsScene *scene;

    Network *network = nullptr;
    netStruct structure;

    Ui::MainWindow *ui;

    void create_data();

public slots:
    void show();
    void start();
    void update();

private:
    QTimer *frames = new QTimer();
    std::vector<TrainingData> data;
    ThreadManager *tm;
};
#endif // MAINWINDOW_H
