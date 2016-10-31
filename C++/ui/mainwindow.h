#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <algorithm>
#include <string>
#include <QMainWindow>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QFileDialog>
#include "ui/traceworker.h"
#include "node/scene.h"
#include "tracer/tracer.h"
#include "image/color.h"


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
    void browse_scene_triggered();
    void reload_scene();
    void camera_changed(int index);
    void radius_changed(double value);
    void reflection_changed(int value);
    void roughness_changed(int value);
    void diffuse_changed();
    void emission_changed();
    void selection_changed(std::size_t node_index);

private:
    std::string _scene_filepath;
    Ui::MainWindow *ui;
    bool signals_suspended = false;
};

#endif // MAINWINDOW_H
