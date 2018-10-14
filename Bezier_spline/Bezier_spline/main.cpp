#include "plot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Plot w(QPoint(5,5), QPoint(350,350),0);
    w.show();

    return a.exec();
}
