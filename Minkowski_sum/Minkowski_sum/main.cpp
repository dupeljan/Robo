#include "widget.h"
#include <QApplication>
#include "calculation.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    Calculation c;
    w.add_points(c.get_result(),QColor(128,194,255));
    w.add_points(c.get_obstacle(),QColor(0,128,255));
    w.add_points(c.get_robot(),QColor(255,0,0));
    w.show();

    return a.exec();
}
