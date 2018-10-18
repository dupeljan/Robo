#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <utility>
#define POINTS_COUNTS 8
#define FIRST_POINT 7


namespace Ui {
class graph;
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
    int parent;   // Путь в начало
    std::vector < std::pair < int, int > > neigbors;  // Список соседей: вес ребра,  номер
};

class graph : public QWidget
{
    Q_OBJECT
private:
 std::vector < int > path;
 int path_len;
 std::vector < std::vector < int > > adjacency_matrix ;
 int Colors[POINTS_COUNTS];
 int firstPoint;
public:
    explicit graph(QWidget *parent = 0);
    ~graph();

private:
    Ui::graph *ui;
    void output_weight();
    void nearles_neighbors();
    void output_result();
};

#endif // GRAPH_H
