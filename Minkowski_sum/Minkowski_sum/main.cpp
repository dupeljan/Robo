#include "widget.h"
#include <QApplication>
#include "calculation.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    Calculation c;
    std::set < std::pair < int , int > > res = c.get_result();
    w.set_points(res);
    w.show();

    return a.exec();
}
