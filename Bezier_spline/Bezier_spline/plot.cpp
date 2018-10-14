#include "plot.h"
#include "ui_plot.h"
#include <QPainter>
#include <QTime>
#include <QPolygon>
#include <iostream>
#include <utility>
#include <math.h>
#include <algorithm>
// Funct for using QSet<QPoint>
inline uint qHash (const QPoint & key){
    return qHash (QPair<int,int>(key.x(), key.y()) );
}


Plot::Plot(QPoint p_startPoint, QPoint p_targetPoint,QWidget *parent /* = 0*/) :
    QWidget(parent),
    ui(new Ui::Plot),
    startPoint(p_startPoint),
    targetPoint(p_targetPoint)
{
    ui->setupUi(this);
}

Plot::~Plot()
{
    delete ui;
}

void Plot::generate_random_points_vect(int count, double delta, QColor color){
    // Get all points

    QSet < QPoint > free_points ;
    for( int x = 0; x <= SCR_LEN_X;x++)
        for ( int y = 0; y <= SCR_LEN_Y; y++)
            free_points.insert(QPoint(x,y));
    //delete all blockade points
    // Now we havn't  blokated points
    // Create random set
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    materialPoints.clear();
    bool skip = false;

    for( int i = 0 ; i < count && free_points.size();){
        // Compute rand_number from free_set
        int rand_number = qrand() % free_points.size();
        // Get point N rand_number from free_set
        QPoint cur_point = *std::next(free_points.begin(),rand_number);
        // Expect lenght between points
        for( QVector < QPoint > :: iterator it = materialPoints.begin(); it != materialPoints.end() && !skip; it++)
            if ( sqrt( pow(it->x() - cur_point.x() , 2) + pow(it->y() - cur_point.y() , 2) ) <= delta)
                skip = true;

        if( ! skip ){
            materialPoints.push_back(cur_point);
            i++;
        }
        free_points.remove(cur_point);
        skip = false;

    }
    //Draw it
    //set_source.push_back(make_pair(material_points, color));
    std::cout << materialPoints.size() << " random points found\n" << free_points.size() << " free elements left";
}

void Plot::generate_example_points_vect(){
    materialPoints.push_back(QPoint(0,200));
    materialPoints.push_back(QPoint(100,100));
    materialPoints.push_back(QPoint(200,200));
    materialPoints.push_back(QPoint(300,100));
    materialPoints.push_back(QPoint(400,200));

}
void Plot::createBezierSpline(){
   //insertStartTargetPoints();
    const double delta_t = 1e-2;
    for(int i = 0; i < /* point before last */ materialPoints.size() - 2 ; i++ )
      // spline.push_back(middle(i));
         for(double t = 0; t <= 1; t+= delta_t )
            spline.push_back( QPoint( B( middle(i).x() , materialPoints[i+1].x() , middle(i+1).x() , t), B( middle(i).y() , materialPoints[i+1].y() , middle(i+1).y() , t ) ) );

}

void Plot::paintEvent(QPaintEvent *event){
     QPainter painter(this);

     // Draw spline
     QPen myPen(QColor(255,0,0));
     painter.setPen(myPen);
     painter.drawPolyline(spline.data(),spline.size());

     // Draw material points
     myPen.setColor(QColor(0,0,255));
     painter.setPen(myPen);
     painter.drawPolyline(materialPoints.data(), materialPoints.size());
}


void Plot::insertStartTargetPoints(){
    // Sort material Points
    std::sort(materialPoints.begin(),materialPoints.end() ,customLess );
// Insert start Point
    //if not find
    if( std::find(materialPoints.begin(), materialPoints.end(), startPoint) == materialPoints.end())
        materialPoints.insert(materialPoints.begin(),startPoint);
    else {// change elem
        std::iter_swap(materialPoints.begin(),std::find(materialPoints.begin(), materialPoints.end(), startPoint) );
        // Sort once again
        std::sort(materialPoints.begin() + 1,materialPoints.end(), customLess);
    }
// Insert target Point
    //if not find
    if( std::find(materialPoints.begin(), materialPoints.end(), targetPoint) == materialPoints.end())
        materialPoints.push_back(startPoint);
    else {// change elem
        std::iter_swap(/*last elem*/materialPoints.end() - 1,std::find(materialPoints.begin(), materialPoints.end(), targetPoint) );
        // Sort once again
        std::sort(materialPoints.begin() + 1,/*before last*/materialPoints.end() -2 , customLess);
    }

}

QPoint Plot::middle(int i){
    if (i < 0 || i >= materialPoints.size() - 1 ){
        /*ERROR*/
        std::cout << "\nError";
        return QPoint(0,0);
   }
    if ( i == 0)
        return materialPoints[0];
    if ( i == /*before last point*/ materialPoints.size() - 2 )
        return materialPoints[i+1];
    return QPoint( ( materialPoints[i].x() + materialPoints[i+1].x() ) / 2 ,  ( materialPoints[i].y() + materialPoints[i+1].y() ) / 2 );
}
