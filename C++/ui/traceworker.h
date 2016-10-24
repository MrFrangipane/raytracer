#ifndef TRACEWORKER_H
#define TRACEWORKER_H

#include <memory>
#include <QObject>
#include <QString>
#include "node/scene.h"
#include "image/pixel.h"
#include "tracer/tracer.h"


class TraceWorker : public QObject
{
    Q_OBJECT
public:
    // Members
    std::size_t width;
    std::size_t height;
    std::size_t pixel_count;
    std::unique_ptr<frangiray::Tracer> tracer;

    // Constructors
    explicit TraceWorker(const std::shared_ptr<frangiray::Scene> &scene_, QObject *parent = 0);

public slots:
    void process();

signals:
    void finished();
    void error(QString error);

private:
    std::shared_ptr<frangiray::Scene> _scene;
};

#endif // TRACEWORKER_H
