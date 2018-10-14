#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QSet>
#include <QVector>
#include <QPoint>
#include <QColor>

namespace Ui {
class Plot;
}

class Plot : public QWidget
{
    Q_OBJECT
    QVector < QPoint > materialPoints;
    QSet < QPoint > spline;
    QPoint startPoint, targetPoint;

public:
    explicit Plot(QWidget *parent = 0);
    ~Plot();
    void generate_random_points_set(int count, double delta, QColor color);
    void createBezierSpline(QPoint p_startPoint, QPoint p_targetPoint);
private:
    Ui::Plot *ui;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // PLOT_H
