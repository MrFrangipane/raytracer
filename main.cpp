#include "mainwindow.h"
#include <QApplication>
#include <vector>
#include <memory>
#include "buffer.h"


int main(int argc, char *argv[])
{
    // Pixel Buffer as shared pointer
    std::shared_ptr<raytracer::Buffer> buffer = std::shared_ptr<raytracer::Buffer>(new raytracer::Buffer(1280, 720));

    QApplication a(argc, argv);
    MainWindow w(buffer);
    w.show();

    return a.exec();
}
