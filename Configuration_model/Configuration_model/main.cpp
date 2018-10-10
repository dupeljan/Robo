#include "environment.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Environment e(QRect(10,10,300,200),QColor(255,0,0),0);
    e.add_circle(QPoint(300,300),50,QColor(0,255,0));
    e.add_rectange(QRect(50,50,100,100),QColor(0,0,255));
    e.add_hollow_rect(QRect(1,1,399,399),QColor(255,255,0));

    e.show();

    return a.exec();
}
