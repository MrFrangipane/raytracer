#include "viewport.h"


// Constructor
Viewport::Viewport(QWidget *parent)
    : QWidget(parent)
{
    // Init size
    set_resolution(width, height);

    // Set Mouse Tracking
    setMouseTracking(true);
}


// Set Buffer
void Viewport::set_buffer(const std::shared_ptr<frangiray::Buffer> tracer_buffer_)
{
    tracer_buffer = tracer_buffer_;
}


// Set Scene
void Viewport::set_scene(const std::shared_ptr<frangiray::Scene> scene_)
{
    scene = scene_;
}


// Set Resolution
void Viewport::set_resolution(size_t width_, size_t height_)
{
    // Update Members
    width = width_;
    height = height_;
    pixel_count = width_ * height_;

    // Init Buffer
    image_buffer.clear();
    image_buffer.reserve(pixel_count);
    for (int i=0; i < pixel_count; i++)
    {
        image_buffer.emplace_back(0);
    }

    // Update Gui
    this->setFixedWidth((int)width);
    this->setFixedHeight((int)height);
}


// Update Image
void Viewport::update_image()
{
    // Test Selection Overlay
    for (int i=0; i < tracer_buffer->pixels.size(); i++)
    {
        // Hover
        if (tracer_buffer->pixels[i]->object_id == (hovered_node_index - 1) && i % 3 == 0)
        {
            image_buffer[i] = qRgba(255, 255, 0, 255);
        }
        // No Hover
        else
        {
            image_buffer[i] = *tracer_buffer->integers[i];
        }
    }

    // Repaint
    update();
}


QString Viewport::tooltip_text()
{
    std::size_t x = mapFromGlobal(QCursor::pos()).x();
    std::size_t y = mapFromGlobal(QCursor::pos()).y();
    std::size_t i = x + y * width;
    std::size_t selected_node_index = tracer_buffer->pixels[i]->object_id + 1;
    return QString::fromStdString(scene->node_at(selected_node_index - 1)->name);
}


// Paint Event
void Viewport::paintEvent(QPaintEvent *event)
{
    QImage image = QImage(
        reinterpret_cast<uchar*>(&image_buffer.front()),
        (int)width, (int)height,
        QImage::Format_ARGB32
    );

    QPainter painter;
    painter.begin(this);
    painter.drawImage(event->rect(), image);
    painter.end();
}


// Mouse Move Event
void Viewport::mouseMoveEvent(QMouseEvent* event)
{
    if (event->modifiers() == Qt::AltModifier)
    {
        // Update Selected Node's index
        hovered_node_index = tracer_buffer->pixels[event->pos().x() + event->pos().y() * width]->object_id + 1;
        // Redraw
        update_image();
    }
    else
    {
        hovered_node_index = 0;
    }
}

// Mouse Press Event
void Viewport::mousePressEvent(QMouseEvent *event)
{
    // Selection Change
    if (event->modifiers() == Qt::AltModifier)
    {
        emit selection_changed(hovered_node_index - 1);
    }
    // Tooltip
    else
    {
        QToolTip::showText(
            event->globalPos(),
            tooltip_text(),
            this,
            rect()
        );
    }
}


// Key Press Event
void Viewport::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::AltModifier)
    {
        emit selection_changed(hovered_node_index - 1);
    }
}


// Leave Event
void Viewport::leaveEvent(QEvent *)
{
    // No Selection
    hovered_node_index = 0;
    // Redraw
    update_image();
}
