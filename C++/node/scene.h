#ifndef SCENE_H
#define SCENE_H

#include <mutex>
#include <vector>
#include <memory>
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

    size_t node_count();

    void add_node(std::shared_ptr<AbstractNode> &node);

    void remove_node(const size_t node_index);

    std::shared_ptr<AbstractNode> node_at(const size_t index);

    void load_from_file(const char* filepath);

private:
    mutable std::shared_mutex _lock;

    std::vector<std::shared_ptr<AbstractNode>> _nodes;
};

}

#endif // SCENE_H
