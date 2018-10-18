#include "graph.h"
#include "ui_graph.h"
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


graph::graph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::graph)
{
    ui->setupUi(this);

    adjacency_matrix = { { 0  , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
                         { 47 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ,
                         { 44 , 46, 0 , 0 , 0 , 0 , 0 , 0 } ,
                         { 56 , 94, 60, 0 , 0 , 0 , 0 , 0 } ,
                         { 94 , 67, 56,113, 0 , 0 , 0 , 0 } ,
                         { 98 ,111, 65, 66, 91, 0 , 0 , 0 } ,
                         { 75 , 76, 33, 70, 55, 39, 0 , 0 } ,
                         { 114, 96, 75,125, 36, 83, 57, 0 }
                       };
    n = adjacency_matrix.size();

    for( int i = 0; i < n; i++)
        for( int j = i + 1 ; j < n; j++)
            adjacency_matrix[i][j] = adjacency_matrix[j][i];



    //Помечаем все вершины как непросмотренные
    for (int i = 0; i < n ; i++)
        Colors[i] = 0;
    nearles_neighbors();
    for ( auto x : path)
        std::cout<<x+1<<' ';
    std::cout<<path_len<<'\n';
}



void graph::nearles_neighbors(){
    int current = FIRST_POINT;
    path_len = 0;
    for ( int visited = 0 ; visited < n ; visited++){
        Colors[current] = 1;
        path.push_back(current);
        int localMin = INT_MAX;
        int new_current;

        for( int i = 0; i < n ; i++)
            if( !Colors[i] && adjacency_matrix[current][i] < localMin ){
                new_current = i;
                localMin = adjacency_matrix[current][i];
            }
        current = new_current;
        path_len += ( visited != n -1 )? localMin :  adjacency_matrix[current][FIRST_POINT];

    }
    path.push_back(FIRST_POINT);
}

graph::~graph()
{
    delete ui;
}
