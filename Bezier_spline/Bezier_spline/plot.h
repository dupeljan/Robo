#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QSet>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <math.h>


#define SCR_LEN_X 400
#define SCR_LEN_Y 400
#define DELTA_T 1e-1

namespace Ui {
class Plot;
}



class Plot : public QWidget
{
    Q_OBJECT
private:
    QVector < QPoint > materialPoints;
    QVector < QPoint > spline;
    QPoint startPoint, targetPoint;
    struct {
            bool operator()(QPoint a, QPoint b) const
            {
                return ( sqrt( pow(a.x(),2) + pow(a.y(),2) ) < sqrt( pow(b.x(),2) + pow(b.y(),2) )  )? true : false;
            }
        } customLess;

public:
    explicit Plot(QPoint p_startPoint, QPoint p_targetPoint,QWidget *parent = 0);
    ~Plot();
    void generate_random_points_vect(int count, double delta, QColor color);
    void generate_example_points_vect();
    void generate_points_for_closed_spline();
    void createCloseCatmullRomSpline();
    void createBezierSpline();
    void createCatmullRomSpline();
    void createCatmullLoopRomSpline();
private:
    Ui::Plot *ui;
    void insertStartTargetPoints(); // Insert start & target points in materialPoints
    // Computing each points of spline
    int B(int P0, int P1, int P2, double t){ return  int((( P0 - 2*P1 +P2 )*pow( t , 2 ) + 2*( P1 - P0 )*t + P0)); }
    // Compute middle point between point i & i+1 in materialPoints
    QPoint middle(int i);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // PLOT_H
