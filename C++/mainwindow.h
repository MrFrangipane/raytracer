#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <thread>
#include <QSlider>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QToolTip>
#include "vector.h"
#include "scene.h"
#include "buffer.h"
#include "qtutils.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int previous_mouse_x;
    int previous_mouse_y;
    QLabel* label_image;
    QSlider* exposure_slider;
    QLabel* label_infos;
    std::shared_ptr<raytracer::Scene> scene;
    std::shared_ptr<raytracer::Buffer> buffer;

    MainWindow(std::shared_ptr<raytracer::Scene> &scene_, std::shared_ptr<raytracer::Buffer> &buffer_, QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

public slots:
    void update_gui();
};

#endif // MAINWINDOW_H