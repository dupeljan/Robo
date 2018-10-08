#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>
#include <set>
#include <utility>

using namespace std;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
private:
    std::set < pair < int , int > > points_instanse;
protected:
    void paintEvent(QPaintEvent *event);
public:
    explicit Widget(QWidget *parent = 0);
    void set_points(set< pair < int , int > > p) { points_instanse = p;}
    ~Widget();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
