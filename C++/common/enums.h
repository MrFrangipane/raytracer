#ifndef ENUMS_H
#define ENUMS_H

namespace frangiray {

enum class BufferType {ObjectId, Depth, Normal, Emission, Albedo, DirectLighting, IndirectLighting, Reflection, Beauty};
#define BUFFER_TYPE_COUNT 9

}

#endif // ENUMS_H
