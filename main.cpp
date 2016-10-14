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


/* TEST MATRIX INVERSION
#include "vector3.h"
#include "matrix.h"
#include <iostream>

int main(int argc, char **argv)
{
    raytracer::Vector3 v(0, 1, 2);
    std::cerr << v << std::endl;
    raytracer::Matrix44 a, b, c;
    c = a * b;

    raytracer::Matrix44 d(0.707107, 0, -0.707107, 0, -0.331295, 0.883452, -0.331295, 0, 0.624695, 0.468521, 0.624695, 0, 4.000574, 3.00043, 4.000574, 1);
    std::cerr << d << std::endl;
    d.invert();
    std::cerr << d << std::endl;

    return 0;
}
*/
