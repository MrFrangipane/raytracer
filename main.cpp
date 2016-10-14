#include "mainwindow.h"
#include <QApplication>
#include <thread>
#include <utility>
#include "color.h"
#include "scene.h"
#include "sphere.h"
#include "renderer.h"


int main(int argc, char *argv[])
{
    // Core Count
    std::size_t core_count = std::thread::hardware_concurrency() - 2;

    // Camera
    raytracer::Matrix44 camera_transform;
    std::unique_ptr<raytracer::Camera> camera(new raytracer::Camera(
        std::move(camera_transform),
        1.0,
        1024,
        576,
        1.0
    ));

    // Sphere
    raytracer::Matrix44 sphere_transform;
    raytracer::Color diffuse;
    diffuse.red = .7;
    diffuse.green = .9;
    diffuse.blue = 0;
    std::unique_ptr<raytracer::Traceable> sphere_1(new raytracer::Sphere(
        std::move(sphere_transform),
        std::move(diffuse),
        raytracer::Color(),
        1.0
    ));

    // Scene
    std::shared_ptr<raytracer::Scene> scene(new raytracer::Scene());
    scene.get()->cameras.push_back(std::move(camera));
    scene.get()->traceables.push_back(std::move(sphere_1));

    // Shared Buffer
    std::shared_ptr<raytracer::Buffer> buffer(new raytracer::Buffer(
        1024,
        576
    ));

    // Render
    std::thread render_thread([=, &buffer, &scene]{
        while (true) {
            raytracer::render(scene, buffer, core_count);}
        }
    );
    render_thread.detach();

    // Application
    QApplication a(argc, argv);
    MainWindow w(buffer);
    w.show();

    // Exit
    return a.exec();
}
