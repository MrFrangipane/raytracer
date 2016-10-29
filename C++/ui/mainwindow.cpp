#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Make Scene
    scene = std::make_shared<frangiray::Scene>();
    scene->load_from_file("C:/scene.json");

    // Trace Worker Thread
    trace_thread = new QThread;
    trace_worker = new TraceWorker(scene);
    trace_worker->moveToThread(trace_thread);

    // Set Viewport Size
    ui->viewport->set_resolution(
        (std::size_t)trace_worker->width,
        (std::size_t)trace_worker->height
    );
    ui->viewport->set_buffer(std::make_shared<frangiray::Buffer>(trace_worker->tracer->buffer));
    ui->viewport->set_scene(scene);

    // Connect Signals
    connect(trace_worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(trace_thread, SIGNAL(started()), trace_worker, SLOT(process()));
    connect(trace_worker, SIGNAL(finished()), trace_thread, SLOT(quit()));
    connect(trace_worker, SIGNAL(finished()), trace_worker, SLOT(deleteLater()));
    connect(trace_worker, SIGNAL(finished()), trace_thread, SLOT(deleteLater()));

    // Connect to slider
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slider1_changed(int)));
    connect(ui->horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(slider2_changed(int)));

    // Start
    trace_thread->start();

    // Refresh Timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_gui()));
    timer->start(500);

    // Update Gui
    update_gui();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slider1_changed(int value)
{
    // Test a la con de bouger la cam
    std::dynamic_pointer_cast<frangiray::Camera>(scene->node_at(0))->fov = value * 0.5;
    trace_worker->tracer->reset_render();

    // Update Gui
    update_gui();
}

void MainWindow::slider2_changed(int value)
{
    // Test a la con de changer l'exposure
    std::dynamic_pointer_cast<frangiray::Camera>(scene->node_at(0))->exposure = value * 0.01;
    trace_worker->tracer->reset_render();

    // Update Gui
    update_gui();
}


void MainWindow::update_gui()
{
    // Update Viewport
    this->ui->viewport->update_image();
}
