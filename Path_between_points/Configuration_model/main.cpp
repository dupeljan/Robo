#include "environment.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Environment e(QRect(-35,-11,36,12),QPoint(100,100),QColor(255,0,0),0);//-35,36,-11,12
    e.add_hollow_rect(QRect(0,0,400,400),QColor(0,0,255));
    e.add_circle(QPoint(150,250),50,QColor(0,0,255));
    e.add_rectange(QRect(280,100,320,250),QColor(0,0,255));
    e.add_rectange(QRect(70,70,130,130),QColor(0,0,255));
    e.calculate_Minkowski_sum(QColor(128,194,255));
    e.generate_random_points_set(30,40,QColor(255,0,0));//30 40
    //e.add_rectange(QRect(200,50,271,73),QColor(255,100,0));
    //e.generate_grid(20,20,QPoint(0,0),QColor(255,0,0));
    e.show();

    return a.exec();
}
