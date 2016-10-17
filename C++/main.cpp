#include "mainwindow.h"
#include <QApplication>
#include <thread>
#include <utility>
#include <fstream>
#include <iostream>
#include <iterator>
#include "color.h"
#include "scene.h"
#include "sphere.h"
#include "renderer.h"

std::unique_ptr<raytracer::Traceable> make_sphere (
        const double radius,
        const int pos_x, const int pos_y, const int pos_z,
        const double diffuse_r, const double diffuse_g, const double diffuse_b,
        const double emission_r, const double emission_g, const double emission_b
    )
{
    // Transform
    raytracer::Matrix44 transform(1, 0, 0, 0,
                                  0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  pos_x, pos_y, pos_z, 1);
    // Colors
    raytracer::Color diffuse;
    diffuse.red = diffuse_r;
    diffuse.green = diffuse_g;
    diffuse.blue = diffuse_b;
    raytracer::Color emission;
    emission.red = emission_r;
    emission.green = emission_g;
    emission.blue = emission_b;

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

    std::unique_ptr<raytracer::Camera> camera(new raytracer::Camera(
        std::move(camera_transform),
        45,
        1024,
        576,
        1.0
    ));

    // Scene
    std::shared_ptr<raytracer::Scene> scene(new raytracer::Scene());

    // Open file
    std::ifstream file("D:/dev/raytracer/raytracer/scene.txt");
    std::string read_line;
    // Prepare Tokens
    std::vector<std::string> tokens;
    // Each Line
    while (std::getline(file, read_line))
    {
        // Prepare Token
        size_t pos = 0;
        std::string token;
        // Each Token
        while ((pos = read_line.find(" ")) != std::string::npos)
        {
            // Get
            token = read_line.substr(0, pos);
            // Store
            tokens.push_back(token);
            // Erase Delimiter
            read_line.erase(0, pos + 1);
        }
        // Store Last
        tokens.push_back(read_line);
    }
    // Each Token
    for (std::vector<std::string>::iterator token = tokens.begin(); token != tokens.end(); ++token)
    {
        std::cout << *token;
        // Sphere
        if (*token == "sphere")
        {
            // Get Values
            token++;
            double radius = std::stod(token->c_str());
            ++token;
            double pos_x = std::stod(token->c_str());
            ++token;
            double pos_y = std::stod(token->c_str());
            ++token;
            double pos_z = std::stod(token->c_str());
            ++token;
            double diffuse_r = std::stod(token->c_str());
            ++token;
            double diffuse_g = std::stod(token->c_str());
            ++token;
            double diffuse_b = std::stod(token->c_str());
            ++token;
            double emission_r = std::stod(token->c_str());
            ++token;
            double emission_g = std::stod(token->c_str());
            ++token;
            double emission_b = std::stod(token->c_str());

            // New Sphere
            std::unique_ptr<raytracer::Traceable> new_sphere = make_sphere(
                radius,
                pos_x, pos_y, pos_z,
                diffuse_r, diffuse_g, diffuse_b,
                emission_r, emission_g, emission_b
            );
            // Add to scene
            scene->traceables.push_back(std::move(new_sphere));
        }
    }

    scene->background.red = .18;
    scene->background.green = .18;
    scene->background.blue = .18;
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
