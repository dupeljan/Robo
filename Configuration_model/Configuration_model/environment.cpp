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

Environment::Environment(QPoint robot_radius, QPoint shift, QColor color, QWidget *parent = 0):
    QWidget(parent),
    ui(new Ui::Environment)
{
    ui->setupUi(this);



    // Add robot in environment
    // Set Robot collor
    robot.second = color;
    // Fill set
    for (int x = -robot_radius ; x <= robot_radius; x++)
        for (int y = -robot_radius ; y <= robot_radius; y++ )
            if ( x * x + y * y <= robot_radius*robot_radius ){
                x += shift;
                y += shift;
                robot.first.insert(QPoint(x,y));
            }
}

Environment::~Environment()
{
    delete ui;
}

void Environment::paintEvent(QPaintEvent *event){

    QPainter painter(this);

}
