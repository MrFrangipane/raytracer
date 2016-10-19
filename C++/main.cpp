#include "mainwindow.h"
#include <QApplication>
#include <thread>
#include <utility>
#include <iostream>
#include <iterator>
#include "scene.h"
#include "matrix.h"
#include "camera.h"
#include "buffer.h"
#include "renderer.h"


int main(int argc, char *argv[])
{
    // Core Count
    std::size_t core_count = std::thread::hardware_concurrency() - 2;

    // Scene
    std::shared_ptr<raytracer::Scene> scene(new raytracer::Scene());

    // Default Camera  (3ds Max default viewport)
    raytracer::Matrix44 camera_transform(
        0.707107, 0.353553, -0.612372, 0,
        -0.707107, 0.353553, -0.612372, 0,
        0.0, 0.866025, 0.5, 0,
        0.0, 0.0, -250.0, 1
    );

    std::unique_ptr<raytracer::Camera> camera(new raytracer::Camera(
        std::move(camera_transform),
        45,
        1024,
        576,
        1.0
    ));

    scene->background.red = .0;
    scene->background.green = .0;
    scene->background.blue = .0;
    scene->cameras.push_back(std::move(camera));

    // Shared Buffer
    std::shared_ptr<raytracer::Buffer> buffer(new raytracer::Buffer(
        1024,
        576
    ));

    // Render
    std::thread render_thread([=, &buffer, &scene]{
        raytracer::render(scene, buffer, core_count);
    });
    render_thread.detach();

    // Application
    QApplication a(argc, argv);
    MainWindow w(scene, buffer);
    w.show();

    // Exit
    return a.exec();
}
