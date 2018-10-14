#include "plot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Plot p(QPoint(5,5), QPoint(350,350),0);
    p.generate_random_points_vect(10,50,QColor(255,0,0));
    p.createBezierSpline();
    p.show();

    return a.exec();
}
