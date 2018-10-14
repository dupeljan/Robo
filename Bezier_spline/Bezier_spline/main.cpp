#include "plot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Plot p(QPoint(0,0), QPoint(400,400),0);
    p.generate_random_points_vect(4,50,QColor(255,0,0));
    //p.generate_example_points_vect();
    p.createBezierSpline();
    p.show();

    return a.exec();
}
