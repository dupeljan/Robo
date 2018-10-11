#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QWidget>
#include <QSet>
#include <QPoint>
#include <QPainter>
#include <QVector>
#include <QColor>
#include <vector>
#include "Delayn.h"

#define SCR_LEN_X 400
#define SCR_LEN_Y 400

namespace Ui {
class Environment;
}

class Environment : public QWidget
{
    Q_OBJECT

private:
    QSet < QPoint > material_points;
    std::vector<Delayn::Triangle<int>> triangles;
    QColor color;
public:
    explicit Environment(QColor color,QWidget *parent = 0);
    void generate_random_points_set(int count, double delta);
    void calculate_Delanuei_triang();


    ~Environment();

private:
    Ui::Environment *ui;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ENVIRONMENT_H
