#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>
#include <set>
#include <utility>
#include <QColor>
#include <vector>

using namespace std;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
private:
    vector < pair < set < pair < int , int > > , QColor > > points_instanse;
    vector < pair < vector < pair < int , int > >, QColor > > polyline_instanse;
protected:
    void paintEvent(QPaintEvent *event);
public:
    explicit Widget(QWidget *parent = 0);
    void add_points_set(set< pair < int , int > > p , QColor color) { points_instanse.push_back( make_pair( p , color) ); }
    void draw_poligon(vector< pair < int , int > > p, QColor color ) { polyline_instanse.push_back(make_pair(p,color))   ;}
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
