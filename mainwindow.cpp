#include <iostream>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QToolTip>
#include "mainwindow.h"


MainWindow::MainWindow(std::shared_ptr<raytracer::Scene> &scene_, const std::shared_ptr<raytracer::Buffer> &buffer_, QWidget *parent)
    : QMainWindow(parent)
{
    // Init Members
    scene = scene_;
    buffer = buffer_;

    // Options
    this->setMouseTracking(true);

    // Refresh Timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_image()));
    timer->start(20);

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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
    {
        scene->cameras[0]->set_fov(scene->cameras[0]->fov + 5);
    }
    if (event->key() == Qt::Key_Right)
    {
        scene->cameras[0]->set_fov(scene->cameras[0]->fov - 5);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    int x = event->pos().x();
    int y = event->pos().y();

    if (x < 0) x = 0;
    if (y < 0) y = 0;

    int width = buffer->width;
    int height = buffer->height;

    if (x > width) x = width;
    if (y > height) y = height;

    QString text;
    text += "pos : [" + QString::number(x) + ", " + QString::number(y) + "]\n";
    text += "r   : " + QString::number(buffer->pixels[y * width + x].red) + "\n";
    text += "g   : " + QString::number(buffer->pixels[y * width + x].green) + "\n";
    text += "b   : " + QString::number(buffer->pixels[y * width + x].blue);

    QToolTip::showText(
        event->globalPos(),
        text,
        this
    );
    QWidget::mouseMoveEvent(event);
}

void MainWindow::update_image() {
    // Get uchar buffer
    std::vector<int> image_buffer = buffer_to_raw(buffer);

    // Create QImage, QPixmap
    QImage image(reinterpret_cast<uchar*>(&image_buffer.front()), buffer->width, buffer->height, QImage::Format_ARGB32);
    QPixmap pixmap = QPixmap::fromImage(image);

    // Update QLabel
    central_label->setPixmap(pixmap);
}
