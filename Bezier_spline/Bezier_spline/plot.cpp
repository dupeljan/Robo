#include "plot.h"
#include "ui_plot.h"
#include <QPainter>

// Funct for using QSet<QPoint>
inline uint qHash (const QPoint & key){
    return qHash (QPair<int,int>(key.x(), key.y()) );
}


Plot::Plot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plot)
{
    ui->setupUi(this);
}

Plot::~Plot()
{
    delete ui;
}

void Plot::paintEvent(QPaintEvent *event){
     QPainter painter(this);
}
