#include <QTimer>
#include "mainwindow.h"


MainWindow::MainWindow(std::shared_ptr<raytracer::Buffer> buffer_, QWidget *parent)
    : QMainWindow(parent)
{
    // Init Members
    buffer = buffer_;

    // Refresh Timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_image()));
    timer->start(1000);

    // Set QLabel as central widget
    central_label = new QLabel(this);
    setCentralWidget(central_label);

    // Create Threads
    render_thread_1 = std::thread(renderr, buffer);
    render_thread_2 = std::thread(renderr, buffer);
    render_thread_3 = std::thread(renderr, buffer);
    render_thread_4 = std::thread(renderr, buffer);
    // Start Threads
    render_thread_1.detach();
    render_thread_2.detach();
    render_thread_3.detach();
    render_thread_4.detach();

    // Update Image
    update_image();

    // Test
    raytracer::Buffer buff(1280, 720);
}

MainWindow::~MainWindow()
{
    // Ya surement des trucs a faire par ici avec les threads
}

void MainWindow::update_image() {
    // Get uchar buffer
    std::vector<int> image_buffer = buffer_to_raw(buffer);

    // Create QImage, QPixmap
    QImage image(reinterpret_cast<uchar*>(&image_buffer.front()), 1280, 720, QImage::Format_ARGB32);
    QPixmap pixmap = QPixmap::fromImage(image);

    // Update QLabel
    central_label->setPixmap(pixmap);
}
