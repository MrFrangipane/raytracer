#ifndef QTUTILS_H
#define QTUTILS_H

#include <memory>
#include "buffer.h"
#include "color.h"

std::vector<int> buffer_to_raw(const std::shared_ptr<raytracer::Buffer> &buffer, const double &factor);

#endif // QTUTILS_H
