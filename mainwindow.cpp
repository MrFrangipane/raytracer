#include <QTimer>
#include "mainwindow.h"


MainWindow::MainWindow(const std::shared_ptr<raytracer::Buffer> buffer_, QWidget *parent)
    : QMainWindow(parent)
{
    // Init Members
    buffer = buffer_;

    // Refresh Timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_image()));
    timer->start(200);

    // Set QLabel as central widget
    central_label = new QLabel(this);
    setCentralWidget(central_label);

    // Update Image
    update_image();
}

MainWindow::~MainWindow()
{
    // Ya surement des trucs a faire par ici avec les threads
}

void MainWindow::update_image() {
    // Get uchar buffer
    std::vector<int> image_buffer = buffer_to_raw(buffer);

    // Create QImage, QPixmap
    QImage image(reinterpret_cast<uchar*>(&image_buffer.front()), buffer.get()->width, buffer.get()->height, QImage::Format_ARGB32);
    QPixmap pixmap = QPixmap::fromImage(image);

    // Update QLabel
    central_label->setPixmap(pixmap);
}
