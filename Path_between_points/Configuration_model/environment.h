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
#include "Delayn.h"

#define SCR_LEN_X 400
#define SCR_LEN_Y 400

using namespace std;
namespace Ui {
class Environment;
}

class Environment : public QWidget
{
    Q_OBJECT
private:
    QSet < pair <QPoint,QPoint> > set_edges;
    QSet < QPoint > material_points;
    vector < QSet < QPoint > >  edge_source;
    vector < pair < QSet < QPoint >, QColor > > set_source;
    pair < QSet <QPoint> , QColor > robot;            //Set A
    pair < QSet < QPoint >, QColor > sum;  // Minkowski sum
    std::vector<Delayn::Edge<float>> edges;
    QSet < QPoint > ocupate_points;
    QPoint startPoint;
    QPoint targetPoint;
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
    void set_path_points(QPoint p_startPoint, QPoint p_targetPoint );
    void triangulate();
private:
    QSet<QPoint> create_rect_edges(QRect rect);
    QSet<QPoint> create_circle_edges(QPoint centre, int radius);
    QSet<QPoint> create_filled_rect(QRect rect);
    QSet<QPoint> create_filled_circle(QPoint centre, int radius);
    bool point_in_obstakle(QPoint p);
    void compute_ocupate_points();
    Ui::Environment *ui;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ENVIRONMENT_H
