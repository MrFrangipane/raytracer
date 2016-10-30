#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "memory"
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
    void radius_changed(double value);
    void reflection_changed(int value);
    void roughness_changed(int value);
    void selection_changed(std::size_t node_index);

private:
    Ui::MainWindow *ui;
    bool signals_suspended = false;
};

#endif // MAINWINDOW_H
