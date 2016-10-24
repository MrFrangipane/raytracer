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
    ui->viewport->setFixedWidth((int)trace_worker->width);
    ui->viewport->setFixedHeight((int)trace_worker->height);

    // Connect Signals
    connect(trace_worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(trace_thread, SIGNAL(started()), trace_worker, SLOT(process()));
    connect(trace_worker, SIGNAL(finished()), trace_thread, SLOT(quit()));
    connect(trace_worker, SIGNAL(finished()), trace_worker, SLOT(deleteLater()));
    connect(trace_worker, SIGNAL(finished()), trace_thread, SLOT(deleteLater()));

    // Connect to slider
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slider_changed(int)));

    // Start
    trace_thread->start();

    // Refresh Timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_gui()));
    timer->start(100);

    // Update Gui
    update_gui();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slider_changed(int value)
{
    // Test a la con de bouger la cam
    scene->node_at(0)->local_to_world[3][2] = value;
    trace_worker->tracer->reset_render();

    // Update Gui
    update_gui();
}



void MainWindow::update_gui()
{
    // Get uchar buffer
    int increment = 1;
    std::size_t pixel_count = trace_worker->pixel_count;
    std::vector<int> int_buffer(pixel_count);

    if (trace_worker->tracer->is_over_rendering) increment = 5;


    // Each pixel
    for (int pixel_index = 0; pixel_index < pixel_count; pixel_index += increment) {
        // Assign
        int_buffer[pixel_index] =  qRgba(
            trace_worker->tracer->buffer.pixels[pixel_index]->beauty.r,
            trace_worker->tracer->buffer.pixels[pixel_index]->beauty.g,
            trace_worker->tracer->buffer.pixels[pixel_index]->beauty.b,
            255
        );
    }

    // Create QImage, QPixmap
    QImage image(reinterpret_cast<uchar*>(
        &int_buffer.front()),
        (int)trace_worker->width,
        (int)trace_worker->height,
        QImage::Format_ARGB32
    );
    QPixmap pixmap = QPixmap::fromImage(image);

    // Update Viewport
    ui->viewport->setPixmap(pixmap);
}
