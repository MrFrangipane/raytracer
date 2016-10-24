#include "scene.h"

namespace frangiray {

Scene::Scene() {}

// Node Count
size_t Scene::node_count()
{
    // Read Lock
    f_shared_lock lock(_lock);

    return _nodes.size();
}


// Add Node
void Scene::add_node(std::shared_ptr<AbstractNode> &node)
{
    // Write Lock
    f_unique_lock lock(_lock);

    _nodes.push_back(node);
}


// Remove Node
void Scene::remove_node(const size_t node_index)
{
    // Write Lock
    f_unique_lock lock(_lock);

    _nodes.erase(_nodes.begin() + node_index);
}


// Node At
std::shared_ptr<AbstractNode> Scene::node_at(const size_t index)
{
    // Read Lock
    f_shared_lock lock(_lock);

    return _nodes.at(index);
}

// Load File
void Scene::load_from_file(const char* filepath)
{
    // Write Lock
    f_unique_lock lock(_lock);

    // Camera Count
    int camera_count = 0;


    // Init JSON
    Json::Value root;
    Json::Reader reader;

    // Open and Parse File
    std::ifstream config_doc(filepath, std::ifstream::binary);
    bool parsing_success = reader.parse(config_doc, root);

    // Clear
    _nodes.clear();

    // Each Node
    const Json::Value nodes_definition = root["nodes"];
    for (uint32_t definition_index = 0; definition_index < nodes_definition.size(); ++definition_index)
    {
        // Proxy Value
        Json::Value node_definition = nodes_definition[definition_index];

        // Name
        std::string name = node_definition["name"].asString();

        // Transform
        Json::Value matrix_def = node_definition["transform"];
        Matrix transform(
            matrix_def[0].asDouble(), matrix_def[1].asDouble(), matrix_def[2].asDouble(), matrix_def[3].asDouble(),
            matrix_def[4].asDouble(), matrix_def[5].asDouble(), matrix_def[6].asDouble(), matrix_def[7].asDouble(),
            matrix_def[8].asDouble(), matrix_def[9].asDouble(), matrix_def[10].asDouble(), matrix_def[11].asDouble(),
            matrix_def[12].asDouble(), matrix_def[13].asDouble(), matrix_def[14].asDouble(), matrix_def[15].asDouble()
        );

        // Camera
        if (node_definition["class"].asString() == "camera")
        {
            // Fov
            f_real fov = node_definition["fov"].asDouble();
            // Exposure
            f_real exposure = node_definition["exposure"].asDouble();

            // New Camera
            _nodes.emplace_back(std::make_shared<Camera>(
                name, transform, fov, exposure
            ));

            // Increment counter
            camera_count ++;
        }

        // Sphere
        else if (node_definition["class"].asString() == "sphere")
        {
            // Diffuse Color
            RealColor diffuse;
            diffuse.r = node_definition["diffuse_color"][0].asDouble();
            diffuse.g = node_definition["diffuse_color"][1].asDouble();
            diffuse.b = node_definition["diffuse_color"][2].asDouble();

            // Emission Color
            RealColor emission;
            emission.r = node_definition["emission_color"][0].asDouble();
            emission.g = node_definition["emission_color"][1].asDouble();
            emission.b = node_definition["emission_color"][2].asDouble();

            // Reflection
            f_real reflection_amount = node_definition["reflection_amount"].asDouble();
            f_real reflection_roughness = node_definition["reflection_roughness"].asDouble();

            // Radius
            f_real radius = node_definition["radius"].asDouble();

            // New Sphere
            _nodes.emplace_back(std::make_shared<Sphere>(
                name, transform, diffuse, emission, reflection_amount, reflection_roughness, radius
            ));
        }
    }

    // If no camera
    if (camera_count == 0)
    {
        // Create Default
        _nodes.emplace_back(std::make_shared<Camera>("default_camera"));
    }
}

}
