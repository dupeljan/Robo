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
#include <QMouseEvent>
#include <math.h>
#include "Delayn.h"

#define SCR_LEN_X 400
#define SCR_LEN_Y 400
#define PATH_WIDHT 3

using namespace std;
namespace Ui {
class Environment;
}

enum colors {
    not_visited,
    visited,
    blocked,
};

struct vertex //Вершина
{
    colors color;
    int  weight;  //Вес вершины
    int parent;   // Путь в начало
    std::vector < std::pair < int, int > > neigbors;  // Список соседей: вес ребра,  номер
};

class Environment : public QWidget
{
    Q_OBJECT
private:
    std::vector < vertex > graph;
    std::vector < QPoint > shortest_path;
    //
    QSet < pair <QPoint,QPoint> > set_edges;
    QSet < QPoint > material_points;
    vector < QSet < QPoint > >  edge_source;  // Ребра прямоугольных obstacles
    vector < pair < QSet < QPoint >, QColor > > set_source;
    pair < QSet <QPoint> , QColor > robot;            //Set A
    pair < QSet < QPoint >, QColor > sum;  // Minkowski sum
    std::vector<Delayn::Edge<float>> edges;
    QSet < QPoint > ocupate_points;
    QPoint startPoint;
    QPoint targetPoint;
    bool startPointSet;
    struct TpointsWrap{
        bool removeStartPoint;
        bool removeTargetPoint;
        Delayn::Edge<float> startEdge, targetEdge;
        TpointsWrap(){
            removeStartPoint = false;
            removeTargetPoint = false;
            startEdge = Delayn::Edge<float>(Delayn::Point<float>(0,0) , Delayn::Point<float>(0,0) );
            targetEdge = Delayn::Edge<float>(Delayn::Point<float>(0,0) , Delayn::Point<float>(0,0) );
        }

    } pointsWrap; // Necessary or not delete points from graph
public:
    explicit Environment(int robot_radius, QPoint shift, QColor color, QWidget *parent /*= 0*/);
    explicit Environment(QRect rect,QPoint shift, QColor color, QWidget *parent = 0);
    ~Environment();
    void add_rectange(QRect rect, QColor color);
    void add_circle(QPoint centre,int radius , QColor color);
    void add_hollow_rect(QRect rect, QColor color);
    void calculate_Minkowski_sum(QColor color);
    void generate_random_points_set(int count, double delta, QColor color);
    void generate_grid(int len_x, int len_y, QPoint left_corner, QColor color);
    void Dijkstra();
    void triangulate();
private:
    QSet<QPoint> create_rect_edges(QRect rect);
    QSet<QPoint> create_circle_edges(QPoint centre, int radius);
    QSet<QPoint> create_filled_rect(QRect rect);
    QSet<QPoint> create_filled_circle(QPoint centre, int radius);
    QSet<QPoint> create_material_line(int x0, int y0, int x1, int y1);
    QPoint get_nearest_point(QPoint newPoint);
    bool point_in_obstakle(QPoint p);
    bool line_in_obstakle(QSet <QPoint> s);
    void compute_ocupate_points();
    void graph_init();
    void extend_graph();
    void squeeze_graph();
    void compute_shortest_path();
    int weight(Delayn::Edge<float> edge) { return sqrt( pow(edge.p1.x - edge.p0.x , 2) + pow(edge.p1.y - edge.p0.y , 2) );}
    int length(QPoint a, QPoint b) { return sqrt( pow(a.x() - b.x() , 2) + pow(a.y() - b.y() , 2) );}
    Ui::Environment *ui;
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent (QMouseEvent * ev);
};

#endif // ENVIRONMENT_H
