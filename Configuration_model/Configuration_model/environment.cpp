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

Environment::Environment(int robot_radius, QPoint shift, QColor color, QWidget *parent /* = 0*/):
    QWidget(parent),
    ui(new Ui::Environment)
{
    ui->setupUi(this);

/*

    // Add robot in environment
    // Set Robot collor
    robot.second = color;
    // Fill set
    for (int x = -robot_radius ; x <= robot_radius; x++)
        for (int y = -robot_radius ; y <= robot_radius; y++ )
            if ( x * x + y * y <= robot_radius*robot_radius ){
                x += shift.x();
                y += shift.y();
                robot.first.insert(QPoint(x,y));
            }
*/
}

Environment::~Environment()
{
    delete ui;
}


void Environment::add_rectange(QRect rect, QColor color){
    set < QPoint > rectangle;
    //Create rectanle edges
    for( int x = rect.x() ; x <= rect.width(); x++ ){
        rectangle.insert(QPoint(x,rect.y()));
        rectangle.insert(QPoint(x,rect.height()));
    }

    for( int y = rect.y() ; y <= rect.height(); y++ ){
        rectangle.insert(QPoint(rect.x(),y));
        rectangle.insert(QPoint(rect.height(),y));
    }
    //Add rect edges
    set_source.push_back(make_pair(rectangle,color));
    //Add rect polygon
    rectangle_polygon.push_back(make_pair(rect,color));

}

void Environment::add_circle(QPoint centre, int radius, QColor color){
    set < QPoint > circle;
    //Create cicle edge
    for( int x = centre.x() - radius; x <= centre.x() + radius; x++)
        for( int y = centre.y() - radius; y <= centre.y() + radius; y++)
            if( x*x + y*y == radius*radius )
                circle.insert(QPoint(x,y));
    //Add cicrle set
    set_source.push_back(make_pair(circle,color));
    //Add cicle polygon
    circle_polygon.push_back(make_tuple(centre,radius,color));

}

void Environment::paintEvent(QPaintEvent *event){

    QPainter painter(this);

}
