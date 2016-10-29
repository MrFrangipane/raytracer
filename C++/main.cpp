#include "ui/mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    // Qt
    QApplication application(argc, argv);
    MainWindow main_window;
    main_window.show();

    // Run & Exit
    return application.exec();
}
