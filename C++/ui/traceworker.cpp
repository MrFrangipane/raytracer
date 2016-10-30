#include "traceworker.h"


// Constructor
TraceWorker::TraceWorker(const std::shared_ptr<frangiray::Scene> &scene_, QObject *parent) : QObject(parent)
{
    width = 1024;
    height = 576;
    pixel_count = width * height;
    _scene = scene_;
    tracer = std::make_unique<frangiray::Tracer>(_scene, width, height);
}


// Process
void TraceWorker::process() {
    tracer->render();
    emit finished();
}
