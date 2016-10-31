#ifndef SCENE_H
#define SCENE_H

#include <mutex>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <shared_mutex>
#include "json.h"
#include "node/abstractnode.h"
#include "node/camera.h"
#include "node/sphere.h"

#include <iostream>


namespace frangiray {

class Scene
{
public:
    Scene();

    void load_from_file(std::string filepath);

    std::size_t node_count();
    void add_node(std::shared_ptr<AbstractNode> &node);
    void remove_node(const size_t node_index);
    void set_selected_node_index(std::size_t node_index);
    std::shared_ptr<AbstractNode> node_at(const size_t index);
    std::shared_ptr<AbstractNode> selected_node();

    std::size_t camera_count();
    void add_camera(std::shared_ptr<Camera> &camera);
    void remove_camera(const size_t camera_index);
    void set_current_camera_index(std::size_t camera_index);
    std::shared_ptr<Camera> camera_at(const size_t index);
    std::shared_ptr<Camera> current_camera();

private:
    mutable std::shared_mutex _lock;

    std::vector<std::shared_ptr<AbstractNode>> _nodes;
    std::size_t _selected_node_index = 0;

    std::vector<std::shared_ptr<Camera>> _cameras;
    std::size_t _current_camera_index = 0;
};

}

#endif // SCENE_H
