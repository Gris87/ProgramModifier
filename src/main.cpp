#include "src/main/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Resources);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
