#include "environment.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Environment e(QRect(10,10,300,200),QColor(255,0,0),0);
    e.show();

    return a.exec();
}
