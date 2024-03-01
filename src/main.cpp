#include "mainwindow.h"

#include <QApplication>
#include <QResource>


int main(int argc, char *argv[])
{
    QResource::registerResource(":/resources.qrc");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
