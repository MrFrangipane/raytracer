#include "mainwindow.h"
#include <QApplication>
#include <thread>
#include <utility>
#include "color.h"
#include "scene.h"
#include "sphere.h"
#include "renderer.h"


std::unique_ptr<raytracer::Traceable> make_sphere (
        const double radius,
        const int x, const int y, const int z,
        const double r, const double g, const double b,
        bool is_emissive)
{
    raytracer::Matrix44 transform(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  x, y, z, 1);
    raytracer::Color diffuse;
    diffuse.red = r;
    diffuse.green = g;
    diffuse.blue = b;
    std::unique_ptr<raytracer::Traceable> sphere(new raytracer::Sphere(
        std::move(transform),
        std::move(diffuse),
        raytracer::Color(),
        radius
    ));

    return sphere;
}


int main(int argc, char *argv[])
{
    // Core Count
    std::size_t core_count = std::thread::hardware_concurrency() - 2;

    // Camera  (from 3dsmax '$camera.transform')
    raytracer::Matrix44 camera_transform(-0.868934, 0.494928, 0.0, 0,
                                         -0.141437, -0.248318, 0.958298, 0,
                                         0.474288, 0.832697, 0.285773, 0,
                                         21.6976, 38.0939, 13.0734, 1);

    std::unique_ptr<raytracer::Camera> camera(new raytracer::Camera(
        std::move(camera_transform),
        45,
        1024,
        576,
        1.0
    ));

    // Spheres
    std::unique_ptr<raytracer::Traceable> sphere_0 = make_sphere(5,     0,  0,  0,      .9, .9, .9,     false);
    std::unique_ptr<raytracer::Traceable> sphere_x = make_sphere(2.5,   5,  0,  0,      .9, .1, .1,     true);
    std::unique_ptr<raytracer::Traceable> sphere_y = make_sphere(2.5,   0,  5,  0,      .1, .9, .1,     true);
    std::unique_ptr<raytracer::Traceable> sphere_z = make_sphere(2.5,   0,  0,  5,      .1, .1, .9,     true);

    // Scene
    std::shared_ptr<raytracer::Scene> scene(new raytracer::Scene());
    scene.get()->cameras.push_back(std::move(camera));
    scene.get()->traceables.push_back(std::move(sphere_0));
    scene.get()->traceables.push_back(std::move(sphere_x));
    scene.get()->traceables.push_back(std::move(sphere_y));
    scene.get()->traceables.push_back(std::move(sphere_z));

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
    MainWindow w(scene, buffer);
    w.show();

    // Exit
    return a.exec();
}
