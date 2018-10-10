#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QWidget>
#include <QPoint>
#include <QColor>
#include <vector>
#include <set>
#include <QPolygon>
#include <utility>
#include <QRect>
#include <tuple>
#include <QSet>

using namespace std;
namespace Ui {
class Environment;
}

class Environment : public QWidget
{
    Q_OBJECT
private:
    //vector < tuple < QPoint , int, QColor > > circle_polygon; // vector < pair < centre , rad, color > >
    vector < pair < QSet < QPoint >, QColor > > set_source;
   // vector < pair < QRect , QColor > > rectangle_polygon;
    pair < QSet <QPoint> , QColor > robot;            //Set A
    pair < QSet < QPoint >, QColor > sum;  // Minkowski sum
public:
    explicit Environment(int robot_radius, QPoint shift, QColor color, QWidget *parent /*= 0*/);
    explicit Environment(QRect rect, QColor color, QWidget *parent = 0);
    ~Environment();
    void add_rectange(QRect rect, QColor color);
    void add_circle(QPoint centre,int radius , QColor color);
    void add_hollow_rect(QRect rect, QColor color);
private:
    QSet<QPoint> create_rect_edges(QRect rect);
    QSet<QPoint> create_filled_rect(QRect rect);
    QSet<QPoint> create_filled_circle(QPoint centre, int radius);
    Ui::Environment *ui;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ENVIRONMENT_H
