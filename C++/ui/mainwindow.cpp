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
    connect(ui->viewport, SIGNAL(selection_changed(std::size_t)), this, SLOT(selection_changed(std::size_t)));

    // Connect Signals
    connect(trace_worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(trace_thread, SIGNAL(started()), trace_worker, SLOT(process()));
    connect(trace_worker, SIGNAL(finished()), trace_thread, SLOT(quit()));
    connect(trace_worker, SIGNAL(finished()), trace_worker, SLOT(deleteLater()));
    connect(trace_worker, SIGNAL(finished()), trace_thread, SLOT(deleteLater()));

    // Connect to Widgets
    connect(ui->radius_spinbox, SIGNAL(valueChanged(double)), this, SLOT(radius_changed(double)));
    connect(ui->reflection_slider, SIGNAL(valueChanged(int)), this, SLOT(reflection_changed(int)));
    connect(ui->roughness_slider, SIGNAL(valueChanged(int)), this, SLOT(roughness_changed(int)));

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


// Radius Changed
void MainWindow::radius_changed(double value)
{
    if (signals_suspended) return;
    std::shared_ptr<frangiray::Sphere> sphere = std::static_pointer_cast<frangiray::Sphere>(scene->selected_node());
    sphere->set_radius(value);

    trace_worker->tracer->reset_render();

    // Update Gui
    update_gui();
}


// Reflection Changed
void MainWindow::reflection_changed(int value)
{

}


// Roughness Changed
void MainWindow::roughness_changed(int value)
{

}


// Selection Changed
void MainWindow::selection_changed(std::size_t node_index)
{
    // Get Node
    scene->set_selected_node_index(node_index);
    std::shared_ptr<frangiray::Sphere> sphere = std::static_pointer_cast<frangiray::Sphere>(scene->selected_node());
    // Update Widgets
    signals_suspended = true;
    ui->radius_spinbox->setMaximum(sphere->radius * 1000);
    ui->radius_spinbox->setValue(sphere->radius);
    ui->name->setText(QString::fromStdString(sphere->name));
    signals_suspended = false;
}


void MainWindow::update_gui()
{
    // Update Viewport
    this->ui->viewport->update_image();
}
