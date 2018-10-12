#include "graph.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Graph w(2,5,0);
    w.show();

    //return a.exec();
}
