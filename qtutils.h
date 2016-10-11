#ifndef QTUTILS_H
#define QTUTILS_H

#include <memory>
#include "buffer.h"
#include "pixel.h"

std::vector<int> buffer_to_raw(const std::shared_ptr<raytracer::Buffer> buffer);

#endif // QTUTILS_H
