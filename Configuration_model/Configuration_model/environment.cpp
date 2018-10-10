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

Environment::Environment(int robot_radius, QPoint centre, QColor color, QWidget *parent /* = 0*/):
    QWidget(parent),
    ui(new Ui::Environment)
{
    ui->setupUi(this);



    // Add robot in environment
    // Set Robot collor
    robot.second = color;
    // Fill set
   robot.first = create_filled_circle(centre,robot_radius);
}
Environment::Environment(QRect rect, QColor color, QWidget *parent /* = 0 */):
    QWidget(parent),
    ui(new Ui::Environment)
{
     ui->setupUi(this);
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

}

void Environment::add_circle(QPoint centre, int radius, QColor color){

    //Add cicrle set
    set_source.push_back(make_pair(create_filled_circle(centre,radius),color));

}

void Environment::add_hollow_rect(QRect rect, QColor color){
    set_source.push_back(make_pair(create_rect_edges(rect),color));
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
