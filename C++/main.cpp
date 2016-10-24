#include "ui/mainwindow.h"
#include <QApplication>
#include <iostream>


int main(int argc, char *argv[])
{
    // Qt
    QApplication application(argc, argv);
    MainWindow main_window;
    main_window.show();

    int return_code = application.exec();

    // Cout Tests
    //std::cout << main_window.scene->node_at(0)->name << "\n";
    //std::cout << main_window.scene->node_at(0)->transform << "\n";

    return return_code;
}
