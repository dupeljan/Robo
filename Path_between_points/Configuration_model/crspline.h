#ifndef CRSPLINE_H
#define CRSPLINE_H
#include <QWidget>
#include <QSet>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <math.h>

#define DELTA_T 1e-1

class CRSpline
{
    struct {
        QPoint PFirst, PLast;
    } derPoints;
    std::vector < QPoint > spline;
public:
    CRSpline( QPoint firstSpline , QPoint lastSpline );
    void createCatmullRomSpline(std::vector<QPoint> materialPoints );
    std::vector<QPoint> get_spline();
};

#endif // CRSPLINE_H
