#include "environment.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Environment e(41,QPoint(100,100),QColor(255,0,0),0);
    e.show();

    return a.exec();
}
