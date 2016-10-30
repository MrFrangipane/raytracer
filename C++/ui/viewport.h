#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <memory>
#include <vector>
#include "image/buffer.h"
#include "node/scene.h"
#include <QString>
#include <QKeyEvent>
#include <QCursor>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QToolTip>
#include <QPaintEvent>
#include <QMouseEvent>


class Viewport : public QWidget
{
    Q_OBJECT
public:
    // Members
    std::size_t width = 0;
    std::size_t height = 0;
    std::size_t pixel_count = 0;
    std::size_t hovered_node_index = 0;
    std::vector<int> image_buffer;
    std::shared_ptr<frangiray::Buffer> tracer_buffer;
    std::shared_ptr<frangiray::Scene> scene;

    // Constructor
    explicit Viewport(QWidget *parent = 0);

    // Methods
    void set_buffer(const std::shared_ptr<frangiray::Buffer> tracer_buffer_);
    void set_scene(const std::shared_ptr<frangiray::Scene> scene_);
    void set_resolution(std::size_t width_, std::size_t height_);
    void update_image();
    QString tooltip_text();

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void leaveEvent(QEvent *);


signals:
    void selection_changed(std::size_t node_index);

public slots:
};

#endif // VIEWPORT_H
