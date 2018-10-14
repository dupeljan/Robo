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

namespace Ui {
class Plot;
}



class Plot : public QWidget
{
    Q_OBJECT
private:
    QVector < QPoint > materialPoints;
    QSet < QPoint > spline;
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
    void generate_random_points_set(int count, double delta, QColor color);
    void createBezierSpline();
private:
    Ui::Plot *ui;
    void insertStartTargetPoints(); // Insert start & target points in materialPoints

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // PLOT_H
