#include "mainwindow.h"
#include <QApplication>
#include <thread>
#include <utility>
#include "color.h"
#include "scene.h"
#include "sphere.h"
#include "renderer.h"

#define SPHERES 8

std::unique_ptr<raytracer::Traceable> make_sphere (
        const double radius,
        const int x, const int y, const int z,
        const double r, const double g, const double b,
        bool is_emissive)
{
    // Transform
    raytracer::Matrix44 transform(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  x, y, z, 1);
    // Colors
    raytracer::Color diffuse;
    raytracer::Color emission;
    if (is_emissive)
    {
        emission.red = r;
        emission.green = g;
        emission.blue = b;
    }
    else
    {
        diffuse.red = r;
        diffuse.green = g;
        diffuse.blue = b;
    }
    // Make
    std::unique_ptr<raytracer::Traceable> sphere(new raytracer::Sphere(
        std::move(transform),
        std::move(diffuse),
        std::move(emission),
        radius
    ));
    // Return
    return sphere;
}


int main(int argc, char *argv[])
{
    // Core Count
    std::size_t core_count = std::thread::hardware_concurrency() - 2;

    // Camera  (from last session)
    raytracer::Matrix44 camera_transform( -0.868934,   0.494928,   0.000000,   0.000000,
                                          -0.141437,  -0.248318,   0.958298,   0.000000,
                                           0.474288,   0.832697,   0.285773,   0.000000,
                                          55.097600,  88.093900,  23.473400,   1.000000);
    // Camera  (from 3dsmax '$camera.transform')
    /*raytracer::Matrix44 camera_transform(-0.868934, 0.494928, 0.0, 0,
                                         -0.141437, -0.248318, 0.958298, 0,
                                         0.474288, 0.832697, 0.285773, 0,
                                         21.6976, 38.0939, 13.0734, 1);*/

    std::unique_ptr<raytracer::Camera> camera(new raytracer::Camera(
        std::move(camera_transform),
        45,
        1024,
        576,
        1.0
    ));

    // Scene
    std::shared_ptr<raytracer::Scene> scene(new raytracer::Scene());

    // Spheres
    for (int row = -SPHERES; row < SPHERES; row++) {
    for (int col = -SPHERES; col < SPHERES; col++)
    {
        std::unique_ptr<raytracer::Traceable> sphere = make_sphere(
            3, row * 5, col * 5, -5 + 1 * ((col + row) % 3), .9, .9, .9, false
        );
        scene->traceables.push_back(std::move(sphere));
    }}

    std::unique_ptr<raytracer::Traceable> sphere_z = make_sphere(
        10,
        0, 0, 5,
        243 / 225.0, 163 / 255.0, 86 / 255.0,
        true
    );
    std::unique_ptr<raytracer::Traceable> sphere_x = make_sphere(
        5,
        15, 5, 1,
        243 / 225.0, 163 / 255.0, 86 / 255.0,
        true
    );
    std::unique_ptr<raytracer::Traceable> sphere_y = make_sphere(
        5,
        5, 15, 1,
        243 / 225.0, 163 / 255.0, 86 / 255.0,
        true
    );
    std::unique_ptr<raytracer::Traceable> sphere_y2 = make_sphere(
        2,
        8, 30, 1,
        86 / 64.0, 163 / 64.0, 243 / 64.0,
        true
    );

    scene->background.red = 200 / 255.0;
    scene->background.green = 200 / 225.0;
    scene->background.blue = 200 / 255.0;
    scene->cameras.push_back(std::move(camera));
    scene->traceables.push_back(std::move(sphere_x));
    scene->traceables.push_back(std::move(sphere_y));;
    scene->traceables.push_back(std::move(sphere_y2));;
    scene->traceables.push_back(std::move(sphere_z));

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
