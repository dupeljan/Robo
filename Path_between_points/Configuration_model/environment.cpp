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
Environment::Environment(QRect rect, QPoint shift, QColor color, QWidget *parent /* = 0 */):
    QWidget(parent),
    ui(new Ui::Environment)
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
    //Got all blockade points
    QSet < QPoint > all_points;
    for ( int i = 0; i < set_source.size(); i++)
        for ( QSet < QPoint > :: iterator it = set_source[i].first.begin(); it != set_source[i].first.end(); it++ )
            all_points.insert(*it);

    // Create random set
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    random_points.clear();
    bool skip = false;

    for( int i = 0 ; i < count;){
        QPoint rand_p;

        rand_p.setX( qrand() % SCR_LEN_X );

        rand_p.setY( qrand() % SCR_LEN_Y );
        // Lenght between points
        for( QSet < QPoint > :: iterator it = random_points.begin(); it != random_points.end() && !skip; it++)
            if ( sqrt( pow(it->x() - rand_p.x() , 2) + pow(it->y() - rand_p.y() , 2) ) <= delta)
                skip = true;

        // Not in substickles
        for( QSet < QPoint > :: iterator it = all_points.begin(); it != all_points.end() && !skip; it++)
            if ( *it == rand_p )
                skip = true;
        if( ! skip ){
            random_points.insert(rand_p);
            i++;
        }

    }
    //Draw it
    set_source.push_back(make_pair(random_points, color ));
    cout<<random_points.size();

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


