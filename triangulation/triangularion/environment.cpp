#include "environment.h"
#include "ui_environment.h"
#include <QTime>
#include <QPoint>
#include <math.h>
#include <iostream>
#include <vector>
#include <QPolygon>
#include "Delayn.h"



// Funct for using QSet<QPoint>
inline uint qHash (const QPoint & key){
    return qHash (QPair<int,int>(key.x(), key.y()) );
}

Environment::Environment(QColor p_color ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Environment),
    color(p_color)
{
    ui->setupUi(this);
}

Environment::~Environment()
{
    delete ui;
}


void Environment::generate_random_points_set(int count, double delta){
    // Get all points
    QSet < QPoint > free_points;
    for( int x = 0; x <= SCR_LEN_X;x++)
        for ( int y = 0; y <= SCR_LEN_Y; y++)
            free_points.insert(QPoint(x,y));

    // Create random set
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    material_points.clear();
    bool skip = false;

    for( int i = 0 ; i < count && free_points.size();){
        // Compute rand_number from free_set
        QPoint cur_point = QPoint ( qrand() % SCR_LEN_X , qrand() % SCR_LEN_Y);
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
   //set_source.push_back(make_pair(material_points, color));
   std :: cout << material_points.size() << " random points found\n" << free_points.size() << " free elements left";
}

void Environment::calculate_Delanuei_triang(){
    // Transfor set to vector
   /* std::vector < double > points;
    for( QSet < QPoint > :: iterator it = material_points.begin(); it != material_points.end(); it++){
        points.push_back(it->x());
        points.push_back(it->y());
    }

    delaunator::Delaunator d(points);
    // Got triangles
    triangles.clear();
    QVector < QPoint > cur_triangle;
    for(std::size_t i = 0; i < d.triangles.size(); i+=3){
        cur_triangle.clear();
        cur_triangle.push_back(QPoint(d.coords[ 2 * d.triangles[i]] , d.coords[2 * d.triangles[i] + 1]));
        cur_triangle.push_back(QPoint(d.coords[2 * d.triangles[i + 1]] , d.coords[2 * d.triangles[i + 1] + 1] ));
        cur_triangle.push_back(QPoint(d.coords[2 * d.triangles[i + 2]] , d.coords[2 * d.triangles[i + 2] + 1] ));
    }
    triangles.push_back(cur_triangle);
    */
    std::vector < Delayn::Point < float > > points;
    // Get appropriate vector
    for( QSet < QPoint > :: iterator it = material_points.begin(); it != material_points.end(); it++)
        points.push_back(Delayn::Point< float >(it->x(),it->y()));
    triangles = Delayn::triangulate<float>(points).edges;




}

void Environment::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    for ( std::vector<Delayn::Edge<float>> :: iterator it = triangles.begin() ; it != triangles.end(); it++){
        QPolygon polygon;
        polygon << QPoint(it->p0.x, it->p0.y );
        polygon << QPoint(it->p1.x, it->p1.y );
        /*
        polygon << QPoint ( it->e0.p0.x , it->e0.p0.y);
        polygon << QPoint ( it->e1.p0.x , it->e1.p1.y);
        polygon << QPoint ( it->e2.p0.x , it->e2.p1.y);
        */

        painter.drawPolyline(polygon);
    }

}
