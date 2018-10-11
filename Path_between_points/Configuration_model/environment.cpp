#include "environment.h"
#include "ui_environment.h"
#include <QPainter>
#include <QBrush>
#include <QRect>
#include <math.h>
#include <QPoint>
#include <QPolygon>
#include <Qt>
#include <algorithm>
#include <QHash>
#include <iostream>
#include <QTime>
#include "Delayn.h"

// Funct for using QSet<QPoint>
inline uint qHash (const QPoint & key){
    return qHash (QPair<int,int>(key.x(), key.y()) );
}

Environment::Environment(int robot_radius, QPoint centre, QColor color, QWidget *parent /* = 0*/):
    QWidget(parent),
    ui(new Ui::Environment),
    startPoint(QPoint(0,0)), targetPoint(QPoint(0,0))
{
    ui->setupUi(this);



    // Add robot in environment
    // Set Robot collor
    robot.second = color;
    // Fill set
   robot.first = create_filled_circle(centre,robot_radius);
}
Environment::Environment(QRect rect, QPoint shift, QColor color, QWidget *parent /* = 0 */):
    QWidget(parent),
    ui(new Ui::Environment),
    startPoint(QPoint(0,0)), targetPoint(QPoint(0,0))
{
     ui->setupUi(this);


    // QRect new_rect( rect.x() + shift.x() , rect.width() + shift.x() , rect.y() + shift.y() ,rect.height() + shift.y() );


    robot.first = create_filled_rect(rect);

    robot.second = color;
}


Environment::~Environment()
{
    delete ui;
}


void Environment::add_rectange(QRect rect, QColor color){

    //Create rectanle edges
    //create_rect_edges(QRect rect)
    //Add rect edges
    set_source.push_back(make_pair(create_filled_rect(rect),color));
    edge_source.push_back(create_rect_edges(rect));

}

void Environment::add_circle(QPoint centre, int radius, QColor color){

    //Add cicrle set
    set_source.push_back(make_pair(create_filled_circle(centre,radius),color));
    edge_source.push_back(create_circle_edges(centre,radius));
}

void Environment::add_hollow_rect(QRect rect, QColor color){
    set_source.push_back(make_pair(create_rect_edges(rect),color));
    edge_source.push_back(create_rect_edges(rect));
}

void Environment::calculate_Minkowski_sum(QColor color){
    sum.second = color;
    for ( int i = 0; i < edge_source.size(); i++ )
        for ( QSet< QPoint >::iterator it = edge_source[i].begin(); it != edge_source[i].end(); it++ )
            for ( QSet<QPoint>::iterator rob_it = robot.first.begin(); rob_it != robot.first.end(); rob_it++ )
                sum.first.insert(QPoint(it->x() + rob_it->x() , it->y() + rob_it->y() ));
    set_source.insert(set_source.begin(),sum);
    //set_source.push_back(sum);
}

void Environment::generate_random_points_set(int count, double delta, QColor color){
    // Get all points

    QSet < QPoint > free_points ;
    for( int x = 0; x <= SCR_LEN_X;x++)
        for ( int y = 0; y <= SCR_LEN_Y; y++)
            free_points.insert(QPoint(x,y));
    //delete all blockade points
    compute_ocupate_points();
   for ( QSet < QPoint > :: iterator it = ocupate_points.begin(); it != ocupate_points.end(); it++ )
            free_points.remove(*it);

    // Create random set
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    material_points.clear();
    bool skip = false;

    for( int i = 0 ; i < count && free_points.size();){
        // Compute rand_number from free_set
        int rand_number = qrand() % free_points.size();
        // Get point N rand_number from free_set
        QPoint cur_point = *next(free_points.begin(),rand_number);
        // Expect lenght between points
        for( QSet < QPoint > :: iterator it = material_points.begin(); it != material_points.end() && !skip; it++)
            if ( sqrt( pow(it->x() - cur_point.x() , 2) + pow(it->y() - cur_point.y() , 2) ) <= delta)
                skip = true;

        if( ! skip ){
            material_points.insert(cur_point);
            i++;
        }
        free_points.remove(cur_point);
        skip = false;

    }
    //Draw it
    set_source.push_back(make_pair(material_points, color));
    cout << material_points.size() << " random points found\n" << free_points.size() << " free elements left";
}

void Environment::generate_grid(int len_x, int len_y, QPoint left_corner, QColor color){
/*  Good params for exiting plot (30,30,QPoint(0,0),QColor(255,0,0))
                                 (20,20,QPoint(0,0),QColor(255,0,0))
*/
    //Got all blockade points
    compute_ocupate_points();
    // Compute grid
    material_points.clear();
    for( int x = left_corner.x(); x <= SCR_LEN_X - left_corner.x() ; x += len_x)
        for( int y = left_corner.y(); y <= SCR_LEN_Y - left_corner.y(); y += len_y)
            if( ocupate_points.find(QPoint(x,y)) == ocupate_points.end())
                material_points.insert(QPoint(x,y));
    // Draw grid
    set_source.push_back(make_pair(material_points,color));
}

void Environment::set_path_points(QPoint p_startPoint, QPoint p_targetPoint){
    startPoint = p_startPoint;
    targetPoint = p_targetPoint;
}

void Environment::triangulate(){

    std::vector < Delayn::Point < float > > points;
    // Get appropriate vector
    for( QSet < QPoint > :: iterator it = material_points.begin(); it != material_points.end(); it++)
        points.push_back(Delayn::Point< float >(it->x(),it->y()));
    // Get triangulated edges
    edges = Delayn::triangulate<float>(points).edges;
    std::vector < Delayn::Edge<float > > edges_buff = edges;
    // Delete edges which cross obstacles


    //void line(int x0, int y0, int x1, int y1, TGAColor color) {
    for ( std::vector<Delayn::Edge<float>> :: iterator it = edges_buff.begin(); it != edges_buff.end(); it++ ){
        int x0(it->p0.x), y0(it->p0.y), x1(it->p1.x), y1(it->p1.x);
        bool steep = false;
        bool drop_edge = false;
        if (std::abs(x0-x1)<std::abs(y0-y1)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0>x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx = x1-x0;
        int dy = y1-y0;
        int derror2 = std::abs(dy)*2;
        int error2 = 0;
        int y = y0;
        for (int x=x0; x<=x1 && !drop_edge; x++) {
            if (steep) {
                //image.set(y, x, color);
                drop_edge = point_in_obstakle(QPoint(y,x));
            } else {
                //image.set(x, y, color);
                drop_edge = point_in_obstakle(QPoint(x,y));
            }
            error2 += derror2;

            if (error2 > dx) {
                y += (y1>y0?1:-1);
                error2 -= dx*2;
            }
        }
    //}
        if ( drop_edge )
            edges.erase(std::remove(edges.begin(), edges.end(), *it), edges.end());
    }


}

void Environment::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    //Draw robot
    QPen myPen(robot.second);
    painter.setPen(myPen);
    for ( QSet <QPoint>::iterator it = robot.first.begin(); it != robot.first.end(); it++ )
         painter.drawPoint(it->x(),it->y());
    // Draw sets
    for ( int i = 0; i < set_source.size(); i++ ){
        QPen myPen(set_source[i].second);
        painter.setPen(myPen);
        for ( QSet< QPoint >::iterator it = set_source[i].first.begin(); it !=set_source[i].first.end(); it++ ){
            painter.drawPoint(it->x(),it->y());
        }
    }

    // Draw edges
    for ( std::vector<Delayn::Edge<float>> :: iterator it = edges.begin() ; it != edges.end(); it++){
        QPolygon polygon;
        polygon << QPoint(it->p0.x, it->p0.y );
        polygon << QPoint(it->p1.x, it->p1.y );

        painter.drawPolyline(polygon);
    }


}

QSet <QPoint> Environment::create_rect_edges(QRect rect){
    QSet < QPoint > rectangle;
    for( int x = rect.x() ; x <= rect.width(); x++ ){
        rectangle.insert(QPoint(x,rect.y()));
        rectangle.insert(QPoint(x,rect.height()));
    }

    for( int y = rect.y() ; y <= rect.height(); y++ ){
        rectangle.insert(QPoint(rect.x(),y));
        rectangle.insert(QPoint(rect.width(),y));
    }
    return rectangle;
}

QSet <QPoint> Environment::create_filled_rect(QRect rect){
    QSet < QPoint > rectangle;
    for( int x = rect.x(); x <= rect.width(); x++)
        for ( int y = rect.y(); y <= rect.height(); y++)
            rectangle.insert(QPoint(x,y));
    return rectangle;

}

QSet <QPoint> Environment::create_filled_circle(QPoint centre, int radius){
    QSet <QPoint> circle;
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++ )
            if ( i * i + j * j <= radius * radius){
                int x = i + centre.x();
                int y = j + centre.y();
                circle.insert(QPoint(x,y));
            }
    return circle;
}

QSet <QPoint> Environment::create_circle_edges(QPoint centre, int radius){
    QSet <QPoint> circle;

    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++ )
            if ( i * i + j * j <= radius * radius ){
                int x = i + centre.x();
                int y = j + centre.y();
                circle.insert(QPoint(x,y));
            }
    return circle;
}

void Environment::compute_ocupate_points(){
   ocupate_points.clear();
   for ( int i = 0; i < set_source.size(); i++)
           for ( QSet < QPoint > :: iterator it = set_source[i].first.begin(); it != set_source[i].first.end(); it++ )
                ocupate_points.insert(*it);

}

bool Environment::point_in_obstakle(QPoint p){
   return (ocupate_points.find(p) != ocupate_points.end())? true : false;
}
