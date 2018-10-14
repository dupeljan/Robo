#include "plot.h"
#include "ui_plot.h"
#include <QPainter>
#include <QTime>
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

void Plot::generate_random_points_set(int count, double delta, QColor color){
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

void Plot::createBezierSpline(){

}
void Plot::paintEvent(QPaintEvent *event){
     QPainter painter(this);
}
bool Plot::pointComparing(QPoint a, QPoint b){
    return ( sqrt( pow(a.x(),2) + pow(a.y(),2) ) > sqrt( pow(b.x(),2) + pow(b.y(),2) )  )? true : false;
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
