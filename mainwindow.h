#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <thread>
#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include "buffer.h"
#include "qtutils.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QLabel  *central_label;
    std::shared_ptr<raytracer::Buffer> buffer;
    std::thread render_thread_1;
    std::thread render_thread_2;
    std::thread render_thread_3;
    std::thread render_thread_4;

    MainWindow(const std::shared_ptr<raytracer::Buffer> buffer_, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void update_image();
};

#endif // MAINWINDOW_H
