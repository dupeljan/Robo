#include "widget.h"
#include "ui_widget.h"
#include <QBrush>
#include <QRect>
#include <math.h>
#include <QPoint>
#include <QPolygon>
#include <Qt>
#include <algorithm>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);
    /*
    //a simple line
    painter.drawLine(1,1,100,100);

    //create a black pen that has solid line
    //and the width is 2.
    QPen myPen(Qt::black, 2, Qt::SolidLine);
    painter.setPen(myPen);
    painter.drawLine(100,100,100,1);

    //draw a point
    myPen.setColor(Qt::red);
    painter.drawPoint(110,110);

    //draw a polygon
    QPolygon polygon;
    polygon << QPoint(130, 140) << QPoint(180, 170)
             << QPoint(180, 140) << QPoint(220, 110)
             << QPoint(140, 100);
     painter.drawPolygon(polygon);

     //draw an ellipse
     //The setRenderHint() call enables antialiasing, telling QPainter to use different
     //color intensities on the edges to reduce the visual distortion that normally
     //occurs when the edges of a shape are converted into pixels
     painter.setRenderHint(QPainter::Antialiasing, true);
     painter.setPen(QPen(Qt::black, 3, Qt::DashDotLine, Qt::RoundCap));
     painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
     painter.drawEllipse(200, 80, 400, 240);
     */
    /*
    QRect rect(1,1,500,500);
    painter.drawRect(rect);
    */
    // Draw sets
    for ( int i = 0; i < points_instanse.size(); i++ ){
        QPen myPen(points_instanse[i].second);
        painter.setPen(myPen);
        for ( set< pair < int , int > >::iterator it = points_instanse[i].first.begin(); it !=points_instanse[i].first.end(); it++ ){
            painter.drawPoint(it->first,it->second);
        }
    }
    // Draw polylines

    for ( int i = 0; i < polyline_instanse.size(); i++ ){
        QPen myPen(polyline_instanse[i].second);
        painter.setPen(myPen);
        painter.setBrush(QBrush(polyline_instanse[i].second, Qt::SolidPattern));
        QPolygon polygon;
        swap(polyline_instanse[i].first[2], polyline_instanse[i].first[3]);
        for ( int j = 0; j < polyline_instanse[i].first.size(); j++ )
            polygon << QPoint( polyline_instanse[i].first[j].first , polyline_instanse[i].first[j].second );
        painter.drawPolygon(polygon);
   }

}
