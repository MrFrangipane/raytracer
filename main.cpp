#include "mainwindow.h"
#include <QApplication>
#include <thread>
#include "renderer.h"



int main(int argc, char *argv[])
{
    // Shared Buffer
    std::shared_ptr<raytracer::Buffer> buffer(new raytracer::Buffer(1024, 576));

    // Core Count
    std::size_t core_count = std::thread::hardware_concurrency() - 2;

    // Render
    std::thread render_thread([=, &buffer]{while (true) {raytracer::render(buffer, core_count);}});
    render_thread.detach();

    // Application
    QApplication a(argc, argv);
    MainWindow w(buffer);
    w.show();

    // Exit
    return a.exec();
}
