#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>
#include <set>
#include <utility>
#include <QColor>

using namespace std;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
private:
    vector < pair < set < pair < int , int > > , QColor > > points_instanse;
protected:
    void paintEvent(QPaintEvent *event);
public:
    explicit Widget(QWidget *parent = 0);
    void add_points(set< pair < int , int > > p , QColor collor) { points_instanse.push_back( make_pair( p , collor) ); }
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
