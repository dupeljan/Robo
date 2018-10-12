#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <utility>
#define FILE_INPUT_PATCH "/home/dupeljan/Richter_robo/Dijkstra/Dijkstra/input.txt"
#define FILE_OUTPUT_PATCH "/home/dupeljan/Richter_robo/Dijkstra/Dijkstra/output.txt"

namespace Ui {
class Graph;
}

enum colors {
    not_visited,
    visited,
    blocked,
};

struct vertex //Вершина
{
    colors color;
    int  weight;  //Вес вершины
    std::vector <int> parent;   // Путь в начало
    std::vector < std::pair < int, int > > neigbors;  // Список соседей: вес ребра,  номер
};

class Graph : public QWidget
{
    Q_OBJECT
private:
    int startPoint, targetPoint;
    std::vector < vertex > graph;
    std::vector <int> path;
    void output_weight();

public:
    explicit Graph(int startpoint, int targetpoint, QWidget *parent = 0);
    ~Graph();

private:
    Ui::Graph *ui;
    void Dijkstra(int v);
    void output_result();
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // GRAPH_H
