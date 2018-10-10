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

// Funct for using QSet<QPoint>
inline uint qHash (const QPoint & key){
    return qHash (QPair<int,int>(key.x(), key.y()) );
}

Environment::Environment(int robot_radius, QPoint shift, QColor color, QWidget *parent /* = 0*/):
    QWidget(parent),
    ui(new Ui::Environment)
{
    ui->setupUi(this);



    // Add robot in environment
    // Set Robot collor
    robot.second = color;
    // Fill set
    for (int i = -robot_radius ; i <= robot_radius; i++)
        for (int j = -robot_radius ; j <= robot_radius; j++ )
            if ( i * i + j * j <= robot_radius * robot_radius ){
                int x = i + shift.x();
                int y = j + shift.y();
                robot.first.insert(QPoint(x,y));
            }

}
Environment::Environment(QRect rect, QColor color, QWidget *parent /* = 0 */):
    QWidget(parent),
    ui(new Ui::Environment)
{
     ui->setupUi(this);

    robot.first = create_rect_edges(rect);
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
    set_source.push_back(make_pair(create_rect_edges(rect),color));
    //Add rect polygon
    rectangle_polygon.push_back(make_pair(rect,color));

}

void Environment::add_circle(QPoint centre, int radius, QColor color){
    QSet < QPoint > circle;
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
