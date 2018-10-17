#include "plot.h"
#include "ui_plot.h"
#include <QPainter>
#include <QTime>
#include <QPolygon>
#include <iostream>
#include <utility>
#include <math.h>
#include <algorithm>
#include "matrixproc.h"
#define PI 3.14159265359
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
    insertStartTargetPoints();
    const double delta_t = 1e-2;
    for(int i = 0; i < /* point before last */ materialPoints.size() - 2 ; i++ )
      // spline.push_back(middle(i));
         for(double t = 0; t <= 1; t+= delta_t )
            spline.push_back( QPoint( B( middle(i).x() , materialPoints[i+1].x() , middle(i+1).x() , t), B( middle(i).y() , materialPoints[i+1].y() , middle(i+1).y() , t ) ) );

}
void Plot::generate_points_for_closed_spline(){
    materialPoints.clear();
    materialPoints.push_back(QPoint(20,5));
    materialPoints.push_back(QPoint(22,12));
    materialPoints.push_back(QPoint(34,17));
    materialPoints.push_back(QPoint(34,24));
    materialPoints.push_back(QPoint(22,36));
    materialPoints.push_back(QPoint(22,25));
    materialPoints.push_back(QPoint(18,20));
    materialPoints.push_back(QPoint(12,20));
    materialPoints.push_back(QPoint(17,17));
    materialPoints.push_back(QPoint(16,10));
    // last Point = first point
    materialPoints.push_back(materialPoints[0]);

    std::for_each(materialPoints.begin() , materialPoints.end(), [](QPoint &p){ p.setX(p.x() * 10); p.setY(p.y() * 10 );} );

}

void Plot::generate_points_for_open_spline(){
    materialPoints.clear();
    const int xShift(100), yShift(200), scale(100);
    for( int i = 0; i < 7; i++)
        materialPoints.push_back( QPoint( ( PI / double(2) ) * i , - cos( ( PI / double(2) ) * i ) ) );

    std::for_each(materialPoints.begin() , materialPoints.end(), [](QPoint &p){ p.setX(p.x() * scale + xShift); p.setY(p.y() * scale + yShift);} );
    derPoints.PFirst = QPoint ( sin(0) * scale + xShift , sin(0) * scale + yShift );
    derPoints.PLast = QPoint ( sin( PI  * 3 * scale ) + xShift , sin(  PI * 3 * scale )+ yShift  ) ;
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
    std::sort(materialPoints.begin(),materialPoints.end() , customLess  );
// Insert start Point
    //if not find
    if( std::find(materialPoints.begin(), materialPoints.end(), startPoint) == materialPoints.end())
        materialPoints.insert(materialPoints.begin(),startPoint);
    else {// change elem
        std::iter_swap(materialPoints.begin(),std::find(materialPoints.begin(), materialPoints.end(), startPoint) );
        // Sort once again
        std::sort(materialPoints.begin() + 1,materialPoints.end(), customLess );
    }
// Insert target Point
    //if not find
    if( std::find(materialPoints.begin(), materialPoints.end(), targetPoint) == materialPoints.end())
        materialPoints.push_back(targetPoint);
    else {// change elem
        std::iter_swap(/*last elem*/materialPoints.end() - 1,std::find(materialPoints.begin(), materialPoints.end(), targetPoint) );
        // Sort once again
        std::sort(materialPoints.begin() + 1,/*before last*/materialPoints.end() -2 , customLess );
    }

}

void Plot::createCatmullRomSpline(){
    //First spline
   const std::vector < std::vector < double > > CFirst={ { 0 , 2 , 0 , 0 } ,
                                                         { 2 , 0 , 0 , 0 } ,
                                                         {-4 ,-5 , 6 ,-1 } ,
                                                         { 2 , 3 ,-4 , 1 } };

   const std::vector < std::vector < double > > CLast={ { 0 , 2 , 0 , 0 } ,
                                                        {-1 , 0 , 1 , 0 } ,
                                                        { 2 ,-6 , 4 ,-2 } ,
                                                        {-1 , 4 ,-3 , 2 } };

   const std::vector < std::vector < double > > C={ { 0 , 2 , 0 , 0 } ,
                                                    {-1 , 0 , 1 , 0 } ,
                                                    { 2 ,-5 , 4 ,-1 } ,
                                                    {-1 , 3 ,-3 , 1 } };

    std::vector < double > Px, Py, Cx, Cy;
    int n = materialPoints.size() - 1;
    for ( int i = 0; i < n  ; i++){

        if ( i == 0 ) {
            Px = { derPoints.PFirst.x() , materialPoints[i].x() , materialPoints[i+1].x() , materialPoints[i+2].x() };
            Py = { derPoints.PFirst.y() , materialPoints[i].y() , materialPoints[i+1].y() , materialPoints[i+2].y() };
            Cx = composition<double>(CFirst,Px);
            Cy = composition<double>(CFirst,Py);
       } else
        if ( i == n - 1 ) {
            Px = { materialPoints[i-1].x() , materialPoints[i].x() , materialPoints[i+1].x() , derPoints.PLast.x() };
            Py = { materialPoints[i-1].y() , materialPoints[i].y() , materialPoints[i+1].y() , derPoints.PLast.y() };
            Cx = composition<double>(CLast,Px);
            Cy = composition<double>(CLast,Py);
        } else {
            Px = { materialPoints[i-1].x() , materialPoints[i].x() , materialPoints[i+1].x() , materialPoints[i+2].x() };
            Py = { materialPoints[i-1].y() , materialPoints[i].y() , materialPoints[i+1].y() , materialPoints[i+2].y() };
            Cx = composition<double>(C,Px);
            Cy = composition<double>(C,Py);
        }
         for ( double t = 0; t <= 1; t += DELTA_T ){
            std::vector<double> T = { 1 , t , t*t , t*t*t };
            spline.push_back(QPoint( composition<double>(T,Cx) / 2 , composition<double>(T,Cy) / 2));
        }
    }
}



void Plot::createCatmullLoopRomSpline(){
    const std::vector < std::vector < double > > C={ { 0 , 2 , 0 , 0 } ,
                                                     {-1 , 0 , 1 , 0 } ,
                                                     { 2 ,-5 , 4 ,-1 } ,
                                                     {-1 , 3 ,-3 , 1 } };
     for( int i = 0; i < materialPoints.size() - 1; i++ ){
         std::vector < double > Px = { materialPoints[ ( i-1 == -1 )? materialPoints.size() - 1 : i-1 ].x() , materialPoints[i].x() , materialPoints[i+1].x() , materialPoints[ (i+2) % materialPoints.size() ].x() };
         std::vector < double > Py = { materialPoints[ ( i-1 == -1 )? materialPoints.size() - 1 : i-1 ].y() , materialPoints[i].y() , materialPoints[i+1].y() , materialPoints[ (i+2) % materialPoints.size() ].y() };
         std::vector < double > Cx = composition<double>(C,Px);
         std::vector < double > Cy = composition<double>(C,Py);
         for ( double t = 0; t <= 1; t += DELTA_T ){
             std::vector<double> T = { 1 , t , t*t , t*t*t };
             spline.push_back(QPoint( composition<double>(T,Cx) / 2 , composition<double>(T,Cy) / 2));
         }
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
