#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QWidget>
#include <QPoint>
#include <QColor>
#include <vector>
#include <set>
#include <QPolygon>
#include <utility>

using namespace std;
namespace Ui {
class Environment;
}

class Environment : public QWidget
{
    Q_OBJECT
private:
    vector < pair < set < QPoint >, QColor > > set_source;
    vector < pair < QPoint , int > > circle_polygon; // vector < pair < centre , rad > >
    vector < QPoint[4] > rectangle_polygon;
    pair < set <QPoint> , QColor > robot;            //Set A
    pair < set < QPoint >, QColor > sum;  // Minkowski sum
public:
    explicit Environment(QPoint robot_radius, QPoint shift, QColor color, QWidget *parent = 0);
    ~Environment();
    void add_rectange(QPoint[4] p, QColor color);
    void add_circle((QPoint centre,int radius , QColor color);
private:
    Ui::Environment *ui;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ENVIRONMENT_H
