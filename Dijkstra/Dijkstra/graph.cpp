#include "graph.h"
#include "ui_graph.h"
#include <QPainter>
#include <QBrush>
#include <QRect>
#include <math.h>
#include <QPoint>
#include <QPolygon>
#include <Qt>
#include <algorithm>
#include <QHash>
#include <iostream>
#include <QTime>
#include <set>
#include <fstream>


// Funct for using QSet<QPoint>
inline uint qHash (const QPoint & key){
    return qHash (QPair<int,int>(key.x(), key.y()) );
}

Graph::Graph(int startpoint, int targetpoint, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Graph),
    startPoint(startpoint),
    targetPoint(targetpoint)

{
    ui->setupUi(this);

    std::fstream file;

    file.open(FILE_INPUT_PATCH,std::fstream::in);
    int n;  //кол-во вершин
    int m;  //кол-во ребер
    file >> n >> m;


    //Рисуем ребра и присваиваем им вес
    graph.resize(n);
    for (int i = 0; i < m; i++) {

        int a, b, w;				//Соединияем вершины a,b ребром веса w
        file >> a >> b >> w;

        graph[a].neigbors.push_back(std::make_pair(w , b));
        graph[b].neigbors.push_back(std::make_pair(w, a ));

    }

    //Помечаем все вершины как непросмотренные
    for (int i = 0; i < n; i++){
        graph[i].color = not_visited;
        //graph[i].parent = begin;  //
    }

    Dijkstra(startpoint);

    //output_weight();
    //output_result(/*start,finish*/);
     output_weight();
    std::cout << "success!" << '\n';
}

Graph::~Graph()
{
    delete ui;
}

void Graph::paintEvent(QPaintEvent *event){
     QPainter painter(this);
}

int min(int a, int b) {
    return (a <= b) ? a : b;
}

void Graph::Dijkstra(int v){
    //void deikstra( int v ){
        std::set < std::pair < int, int > > search; // Вес, номер
        search.insert(std::make_pair(0, v));

        graph[v].weight = 0;
        ///
        graph[v].parent.push_back(  v ) ;
        ///
        std::set < std::pair < int, int > >::iterator it; // Итератор
        while (!search.empty()) {

            //Извлекаем первый элемент множества
            it = search.begin();

            //Получаем указатель на текущую вершину
            vertex *cur_vert = &graph[it->second];

            //Удаляем элемент из множества
            search.erase(it);


            //Блокируем текущую вершину
            cur_vert->color = blocked;

            //Для каждого соседа
            for (int i = 0; i < cur_vert->neigbors.size(); i++) {

                //Получаем указатель на соседа
                vertex *cur_neig = & graph[cur_vert->neigbors[i].second];

                if (cur_neig->color != blocked) {


                    //Вычисляем длинну
                    if (cur_neig->color == not_visited){								//Если не посещена
                        cur_neig->weight = cur_vert->weight + cur_vert->neigbors[i].first; //Вес равен сумме веса текущей вершины и соед. ребра
                        ///
                        cur_neig->parent.push_back( it->second);
                        ///
                    }else{                                                           //Вес сохраняется, если новый больше
                        ///
                        if ( cur_neig->weight > cur_vert->weight + cur_vert->neigbors[i].first )
                            cur_neig->parent = graph[it->second].parent;
                            cur_neig->parent.push_back(it->second);
                        ///
                        cur_neig->weight = min(cur_neig->weight, cur_vert->weight + cur_vert->neigbors[i].first);
                    }
                    //Посещаем вершину
                    cur_neig->color = visited;

                    //Вставляем в множество
                    search.insert(std::make_pair(cur_neig->weight, cur_vert->neigbors[i].second ));// Записываем вес и номер вершины
                }
            }
        }
        //return 0;
    //}
}

void Graph::output_result(){
    //Compute path
   /*
    path.push_back( targetPoint );
    for( int i = 0; path[i] != startPoint ; i++ )
        path.push_back(graph[path[i]].parent);
    */
    //path.push_back(startPoint);
    // Write in file
    std::fstream out;
    out.open(FILE_OUTPUT_PATCH, std::fstream::out);
    for( int i = path.size() - 1; i > -1; i-- )
        out << /*path[i]*/char(65 + path[i]) << ' ';
}
void Graph::output_weight() {

    std::fstream out;
    out.open(FILE_OUTPUT_PATCH, std::fstream::out);
    out << "vertex weight \n";
    for (int i = 0; i < graph.size(); i++){
        out << char(65 + i) << ':' ;
        for( int j = 0 ; j < graph[i].parent.size(); j++ )
            out<< char ( 65 + graph[i].parent[j] ) << ' ';
        out << '\n';
    }

}
