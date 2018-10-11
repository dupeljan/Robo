#include "environment.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Environment w(QColor(0,0,255),0);
    w.generate_random_points_set(100,10);
    w.calculate_Delanuei_triang();
    w.show();

    return a.exec();
}
