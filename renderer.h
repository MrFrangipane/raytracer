#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>
#include <atomic>
#include <future>
#include "buffer.h"

namespace raytracer {

Pixel trace();

void Render(std::shared_ptr<Buffer> buffer, const std::size_t core_count);

} // end namespace

#endif // RENDERER_H
