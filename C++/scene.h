#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <shared_mutex>
#include "color.h"
#include "camera.h"
#include "traceable.h"
#include "sphere.h"

namespace raytracer {

class Scene {
    public:
        // Members
        std::shared_mutex write_mutex;
        Color background;
        std::vector<std::shared_ptr<Camera>> cameras;
        std::vector<std::shared_ptr<Traceable>> traceables;

        // Constructor
        Scene() {}

        std::shared_ptr<Camera> camera_at(const int index)
        {
            // Mutex
             std::shared_lock<std::shared_mutex> lock(write_mutex);
            // Return
            return cameras.at(index);
        }

        std::shared_ptr<Traceable> traceable_at(const int index)
        {
            // Mutex
             std::shared_lock<std::shared_mutex> lock(write_mutex);
            // Return
            return traceables.at(index);
        }

        std::unique_ptr<Traceable> make_sphere (
                const double radius,
                const int pos_x, const int pos_y, const int pos_z,
                const double diffuse_r, const double diffuse_g, const double diffuse_b,
                const double emission_r, const double emission_g, const double emission_b,
                const double reflection_amount, const double reflection_blur
            )
        {
            // Transform
            Matrix44 transform(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                pos_x, pos_y, pos_z, 1
            );
            // Colors
            Color diffuse;
            diffuse.red = diffuse_r;
            diffuse.green = diffuse_g;
            diffuse.blue = diffuse_b;
            Color emission;
            emission.red = emission_r;
            emission.green = emission_g;
            emission.blue = emission_b;

            // Make
            std::unique_ptr<Traceable> sphere(new Sphere(
                std::move(transform),
                std::move(diffuse),
                std::move(emission),
                reflection_amount,
                reflection_blur,
                radius
            ));
            // Return
            return sphere;
        }

        void load_from_file(const char* filepath)
        {
            // Mutex
            std::lock_guard<std::shared_mutex> lock(write_mutex);

            // Open file
            std::ifstream file(filepath);
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
            // Empty Scene
            traceables.clear();
            cameras.clear();
            // Each Token
            for (std::vector<std::string>::iterator token = tokens.begin(); token != tokens.end(); ++token)
            {
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
                    ++token;
                    double reflection_amount = std::stod(token->c_str());
                    ++token;
                    double reflection_blur = std::stod(token->c_str());
                    // New Sphere
                    std::unique_ptr<Traceable> new_sphere = make_sphere(
                        radius,
                        pos_x, pos_y, pos_z,
                        diffuse_r, diffuse_g, diffuse_b,
                        emission_r, emission_g, emission_b,
                        reflection_amount, reflection_blur
                    );
                    // Add to scene
                    traceables.push_back(std::move(new_sphere));
                }
                // Camera
                else if (*token == "camera")
                {
                    // Get Values
                    token++;
                    double fov = std::stod(token->c_str());
                    token++;
                    double aa = std::stod(token->c_str());
                    token++;
                    double ab = std::stod(token->c_str());
                    token++;
                    double ac = std::stod(token->c_str());
                    token++;
                    double ad = std::stod(token->c_str());
                    token++;
                    double ba = std::stod(token->c_str());
                    token++;
                    double bb = std::stod(token->c_str());
                    token++;
                    double bc = std::stod(token->c_str());
                    token++;
                    double bd = std::stod(token->c_str());
                    token++;
                    double ca = std::stod(token->c_str());
                    token++;
                    double cb = std::stod(token->c_str());
                    token++;
                    double cc = std::stod(token->c_str());
                    token++;
                    double cd = std::stod(token->c_str());
                    token++;
                    double da = std::stod(token->c_str());
                    token++;
                    double db = std::stod(token->c_str());
                    token++;
                    double dc = std::stod(token->c_str());
                    token++;
                    double dd = std::stod(token->c_str());

                    // New Camera
                    Matrix44 camera_transform(
                        aa, ab, ac, ad,
                        ba, bb, bc, bd,
                        ca, cb, cc, cd,
                        da, db, dc, dd
                    );
                    std::unique_ptr<Camera> camera(new Camera(
                        std::move(camera_transform),
                        fov,
                        1024,
                        576,
                        1.0
                    ));
                    // Add to scene
                    cameras.push_back(std::move(camera));
                }
            }
        }
};


} // end namespace

#endif // SCENE_H
