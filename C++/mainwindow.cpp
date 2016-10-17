#include "mainwindow.h"


MainWindow::MainWindow(std::shared_ptr<raytracer::Scene> &scene_, std::shared_ptr<raytracer::Buffer> &buffer_, QWidget *parent)
    : QMainWindow(parent)
{
    // Init Members
    scene = scene_;
    buffer = buffer_;

    // Options
    this->setMouseTracking(true);

    // Refresh Timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_gui()));
    timer->start(1000);

    // Layout
    QHBoxLayout* layout = new QHBoxLayout;
    // Image QLabel
    label_image = new QLabel;
    layout->addWidget(label_image);

    // QSlider
    exposure_slider = new QSlider;
    exposure_slider->setMinimum(0);
    exposure_slider->setMaximum(200);
    exposure_slider->setValue(100);
    connect(exposure_slider, SIGNAL(valueChanged(int)), this, SLOT(update_gui()));
    layout->addWidget(exposure_slider);

    // Infos QLabel
    label_infos = new QLabel;
    layout->addWidget(label_infos);

    // Central Widget
    QWidget* central_widget = new QWidget;
    central_widget->setLayout(layout);
    setCentralWidget(central_widget);

    // Update Image
    update_gui();
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

void MainWindow::wheelEvent(QWheelEvent *event)
{
    scene->cameras[0]->set_fov(scene->cameras[0]->fov - event->delta() / 20);
    // Reset Render
    buffer->reset_render();
    update_gui();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    previous_mouse_x = event->pos().x();
    previous_mouse_y = event->pos().y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    int x = event->pos().x();
    int y = event->pos().y();

    int delta_x = (x - previous_mouse_x);
    int delta_y = (y - previous_mouse_y);
    previous_mouse_x = x;
    previous_mouse_y = y;

    // Right Button
    if (event->buttons() == Qt::RightButton)
    {
        scene->cameras[0]->move_by(raytracer::Vector3(0, 0, (double)delta_y / 5));
        // Reset Render
        buffer->reset_render();
        update_gui();
    }
    // Left Button
    else if (event->buttons() == Qt::LeftButton)
    {
        scene->cameras[0]->move_by(raytracer::Vector3((double)delta_x / 5, (double)-delta_y / 5, 0));
        // Reset Render
        buffer->reset_render();
        update_gui();
    }
    // Middle Button
    else if (event->buttons() == Qt::MiddleButton)
    {
        if (x < 0) x = 0;
        if (y < 0) y = 0;

        int width = buffer->width;
        int height = buffer->height;

        if (x > width) x = width;
        if (y > height) y = height;

        QString text;
        text += "pos : [" + QString::number(x) + ", " + QString::number(y) + "]\n";
        text += "r   : " + QString::number(buffer->pixels[y * width + x]->red) + "\n";
        text += "g   : " + QString::number(buffer->pixels[y * width + x]->green) + "\n";
        text += "b   : " + QString::number(buffer->pixels[y * width + x]->blue) + "\n";
        text += "a   : " + QString::number(buffer->pixels[y * width + x]->alpha);

        QToolTip::showText(
            event->globalPos(),
            text,
            this
        );
    }
    // Forward
    QWidget::mouseMoveEvent(event);
}

void MainWindow::update_gui()
{
    // Get uchar buffer
    std::vector<int> int_buffer(buffer->pixel_count);

    double factor = (double)exposure_slider->value() / 100.0;
    //if (buffer->render_iteration / PIXEL_DIVISION > 0) factor /= (buffer->render_iteration / PIXEL_DIVISION);

    for (int pixel_index = 0; pixel_index < buffer->pixel_count; pixel_index++) {
        // Float to 8bit
        int_buffer[pixel_index] =  qRgba(
            (int)((1 - exp(-buffer->pixels[pixel_index]->red * factor)) * 255),
            (int)((1 - exp(-buffer->pixels[pixel_index]->green * factor)) * 255),
            (int)((1 - exp(-buffer->pixels[pixel_index]->blue * factor)) * 255),
            (int)(buffer->pixels[pixel_index]->alpha * 255)
        );
    }

    // Create QImage, QPixmap
    QImage image(reinterpret_cast<uchar*>(&int_buffer.front()), buffer->width, buffer->height, QImage::Format_ARGB32);
    QPixmap pixmap = QPixmap::fromImage(image);

    // Update QLabels
    label_image->setPixmap(pixmap);
    label_infos->setText(QString::number(buffer->render_iteration / PIXEL_DIVISION));
}
