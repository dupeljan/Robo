#include "environment.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Environment e;
    e.show();

    return a.exec();
}
