#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include "ui/traceworker.h"
#include "node/scene.h"
#include "tracer/tracer.h"

#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Members
    std::shared_ptr<frangiray::Scene> scene;
    QThread* trace_thread;
    TraceWorker* trace_worker;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void update_gui();
    void slider1_changed(int value);
    void slider2_changed(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
