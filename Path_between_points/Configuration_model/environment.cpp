#include "environment.h"
#include "ui_environment.h"
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
#include "Delayn.h"

// Funct for using QSet<QPoint>
inline uint qHash (const QPoint & key){
    return qHash (QPair<int,int>(key.x(), key.y()) );
}


Environment::Environment(int robot_radius, QPoint centre, QColor color, QWidget *parent /* = 0*/):
    QWidget(parent),
    ui(new Ui::Environment),
    startPoint(QPoint(0,0)), targetPoint(QPoint(0,0)),
    startPointSet(false)

{
    ui->setupUi(this);



    // Add robot in environment
    // Set Robot collor
    robot.second = color;
    // Fill set
   robot.first = create_filled_circle(centre,robot_radius);
}
Environment::Environment(QRect rect, QPoint shift, QColor color, QWidget *parent /* = 0 */):
    QWidget(parent),
    ui(new Ui::Environment),
    startPoint(QPoint(0,0)), targetPoint(QPoint(0,0)),
    startPointSet(false)
{
     ui->setupUi(this);



    // QRect new_rect( rect.x() + shift.x() , rect.width() + shift.x() , rect.y() + shift.y() ,rect.height() + shift.y() );


    robot.first = create_filled_rect(rect);

    robot.second = color;
}


Environment::~Environment()
{
    delete ui;
}


void Environment::add_rectange(QRect rect, QColor color){

    //Create rectanle edges
    //create_rect_edges(QRect rect)
    //Add rect edges
    set_source.push_back(make_pair(create_filled_rect(rect),color));
    edge_source.push_back(create_rect_edges(rect));

}

void Environment::add_circle(QPoint centre, int radius, QColor color){

    //Add cicrle set
    set_source.push_back(make_pair(create_filled_circle(centre,radius),color));
    edge_source.push_back(create_circle_edges(centre,radius));
}

void Environment::add_hollow_rect(QRect rect, QColor color){
    set_source.push_back(make_pair(create_rect_edges(rect),color));
    edge_source.push_back(create_rect_edges(rect));
}

void Environment::calculate_Minkowski_sum(QColor color){
    sum.second = color;
    for ( int i = 0; i < edge_source.size(); i++ )
        for ( QSet< QPoint >::iterator it = edge_source[i].begin(); it != edge_source[i].end(); it++ )
            for ( QSet<QPoint>::iterator rob_it = robot.first.begin(); rob_it != robot.first.end(); rob_it++ )
                sum.first.insert(QPoint(it->x() + rob_it->x() , it->y() + rob_it->y() ));
    set_source.insert(set_source.begin(),sum);
    //set_source.push_back(sum);
}

void Environment::generate_random_points_set(int count, double delta, QColor color){
    // Get all points

    QSet < QPoint > free_points ;
    for( int x = 0; x <= SCR_LEN_X;x++)
        for ( int y = 0; y <= SCR_LEN_Y; y++)
            free_points.insert(QPoint(x,y));
    //delete all blockade points
    compute_ocupate_points();
   for ( QSet < QPoint > :: iterator it = ocupate_points.begin(); it != ocupate_points.end(); it++ )
            free_points.remove(*it);

    // Create random set
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    material_points.clear();
    bool skip = false;

    for( int i = 0 ; i < count && free_points.size();){
        // Compute rand_number from free_set
        int rand_number = qrand() % free_points.size();
        // Get point N rand_number from free_set
        QPoint cur_point = *next(free_points.begin(),rand_number);
        // Expect lenght between points
        for( QSet < QPoint > :: iterator it = material_points.begin(); it != material_points.end() && !skip; it++)
            if ( sqrt( pow(it->x() - cur_point.x() , 2) + pow(it->y() - cur_point.y() , 2) ) <= delta)
                skip = true;

        if( ! skip ){
            material_points.insert(cur_point);
            i++;
        }
        free_points.remove(cur_point);
        skip = false;

    }
    //Draw it
    set_source.push_back(make_pair(material_points, color));
    cout << material_points.size() << " random points found\n" << free_points.size() << " free elements left";
}

void Environment::generate_grid(int len_x, int len_y, QPoint left_corner, QColor color){
/*  Good params for exiting plot (30,30,QPoint(0,0),QColor(255,0,0))
                                 (20,20,QPoint(0,0),QColor(255,0,0))
*/
    //Got all blockade points
    compute_ocupate_points();
    // Compute grid
    material_points.clear();
    for( int x = left_corner.x(); x <= SCR_LEN_X - left_corner.x() ; x += len_x)
        for( int y = left_corner.y(); y <= SCR_LEN_Y - left_corner.y(); y += len_y)
            if( ocupate_points.find(QPoint(x,y)) == ocupate_points.end())
                material_points.insert(QPoint(x,y));
    // Draw grid
    set_source.push_back(make_pair(material_points,color));
}

void Environment::triangulate(){

    std::vector < Delayn::Point < float > > points;
    // Get appropriate vector
    for( auto point : material_points )
        points.push_back(Delayn::Point< float >(point.x(),point.y()));
    // Get triangulated edges
    edges.clear();
    edges = Delayn::triangulate<float>(points).edges;
    //std::vector < Delayn::Edge<float > > edges_buff = edges;
    // Copy vector to another
    std::vector<Delayn::Edge<float>> buff_edges = edges;
     QSet <QPoint> line;
    /*for( std::vector<Delayn::Edge<float>> :: iterator it = buff_edges.begin(); it != buff_edges.end(); it++ )
        edges_buff.insert(make_pair(QPoint(it->p0.x , it->p0.y),QPoint( it->p1.x, it->p1.y)));
       */
    // Delete edges which cross obstacles
    for (auto edge : buff_edges ){
        line = create_material_line(edge.p0.x, edge.p0.y,edge.p1.x, edge.p1.y);
        //Find obstacle
        if (line_in_obstakle(line))
            edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
    }

    //Draw it
    /*
    for ( QVector < QSet <QPoint>> :: iterator it = lines.begin(); it != lines.end(); it++ )
            set_source.push_back(make_pair(*it,QColor(255,0,0)));
    */

}

int min(int a, int b) {
    return (a <= b) ? a : b;
}

void Environment::Dijkstra(){
    // Create graph
    graph_init();

    int v = std::distance( material_points.begin() , material_points.find( startPoint )  );

    //void deikstra( int v ){
        std::set < std::pair < int, int > > search; // Вес, номер
        search.insert(std::make_pair(0, v));

        graph[v].weight = 0;
        ///
        graph[v].parent =  v ;
        ///
        std::set < std::pair < int, int > >::iterator it; // Итератор
        while (!search.empty()) {

            //Извлекаем первый элемент множества
            it = search.begin();

            //Получаем указатель на текущую вершину
            vertex *cur_vert = &graph[it->second];
            ///
            int cur_vert_numb = it->second;
            ///
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
                        cur_neig->parent = cur_vert_numb;
                        ///
                    }else{                                                           //Вес сохраняется, если новый больше
                        ///
                        if ( cur_neig->weight > cur_vert->weight + cur_vert->neigbors[i].first )
                            cur_neig->parent = cur_vert_numb;

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
        compute_shortest_path();
        update();


}

void Environment::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    //Draw robot
    QPen myPen(robot.second);
    painter.setPen(myPen);
    for ( auto point :  robot.first )
         painter.drawPoint(point.x(),point.y());
    // Draw sets
    for ( int i = 0; i < set_source.size(); i++ ){
        QPen myPen(set_source[i].second);
        painter.setPen(myPen);
        for ( auto point : set_source[i].first ){
            painter.drawPoint(point.x(),point.y());
        }
    }

    // Draw edges
    for ( auto edge : edges ){
        QPolygon polygon;
        polygon << QPoint(edge.p0.x, edge.p0.y);
        polygon << QPoint(edge.p1.x, edge.p1.y);

        painter.drawPolyline(polygon);
    }

    // Draw shortest path
    myPen.setColor(QColor(0,0,255));
    myPen.setWidth(PATH_WIDHT);
    painter.setPen(myPen);
    painter.drawPolyline(shortest_path.data(), shortest_path.size());


}

QSet <QPoint> Environment::create_rect_edges(QRect rect){
    QSet < QPoint > rectangle;
    for( int x = rect.x() ; x <= rect.width(); x++ ){
        rectangle.insert(QPoint(x,rect.y()));
        rectangle.insert(QPoint(x,rect.height()));
    }

    for( int y = rect.y() ; y <= rect.height(); y++ ){
        rectangle.insert(QPoint(rect.x(),y));
        rectangle.insert(QPoint(rect.width(),y));
    }
    return rectangle;
}

QSet <QPoint> Environment::create_filled_rect(QRect rect){
    QSet < QPoint > rectangle;
    for( int x = rect.x(); x <= rect.width(); x++)
        for ( int y = rect.y(); y <= rect.height(); y++)
            rectangle.insert(QPoint(x,y));
    return rectangle;

}

QSet <QPoint> Environment::create_filled_circle(QPoint centre, int radius){
    QSet <QPoint> circle;
    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++ )
            if ( i * i + j * j <= radius * radius){
                int x = i + centre.x();
                int y = j + centre.y();
                circle.insert(QPoint(x,y));
            }
    return circle;
}

QSet <QPoint> Environment::create_circle_edges(QPoint centre, int radius){
    QSet <QPoint> circle;

    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++ )
            if ( i * i + j * j <= radius * radius ){
                int x = i + centre.x();
                int y = j + centre.y();
                circle.insert(QPoint(x,y));
            }
    return circle;
}

void Environment::compute_ocupate_points(){
   ocupate_points.clear();
   for ( int i = 0; i < set_source.size(); i++)
           for ( QSet < QPoint > :: iterator it = set_source[i].first.begin(); it != set_source[i].first.end(); it++ )
                ocupate_points.insert(*it);

}

bool Environment::point_in_obstakle(QPoint p){
   return (ocupate_points.find(p) != ocupate_points.end())? true : false;
}

bool Environment::line_in_obstakle(QSet<QPoint> s){
    //Obstacles must be computed!
    bool result = false;
    for(QSet<QPoint>::iterator it = s.begin(); it != s.end() && !result; it++)
        result = ocupate_points.find(*it) != ocupate_points.end();
    return result;
}

QSet <QPoint> Environment::create_material_line(int x0, int y0, int x1, int y1){
    QSet <QPoint> line;
    bool steep = false;
        if (std::abs(x0-x1)<std::abs(y0-y1)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0>x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int dx = x1-x0;
        int dy = y1-y0;
        int derror2 = std::abs(dy)*2;
        int error2 = 0;
        int y = y0;
        for (int x=x0; x<=x1; x++) {
            if (steep) {
                //image.set(y, x, color);
                line.insert(QPoint(y,x));
            } else {
                //image.set(x, y, color);
                line.insert(QPoint(x,y));

            }
            error2 += derror2;

            if (error2 > dx) {
                y += (y1>y0?1:-1);
                error2 -= dx*2;
            }
        }
        return line;
}


void Environment::graph_init(){
    graph.clear();
    graph.resize(material_points.size());

    for( auto edge : edges){
        int w = weight(edge);
        int a = std::distance( material_points.begin() , material_points.find(QPoint(edge.p0.x , edge.p0.y)) );
        int b = std::distance( material_points.begin() , material_points.find(QPoint(edge.p1.x , edge.p1.y)) );
        graph[a].neigbors.push_back(std::make_pair(w , b));
        graph[b].neigbors.push_back(std::make_pair(w , a));
    }
    for ( auto ver : graph)
        ver.color = not_visited;
}

void Environment::compute_shortest_path(){
    // Get point N rand_number from free_set
    //QPoint cur_point = *next(material_points.begin(),number);
    shortest_path.clear();
    shortest_path.push_back( targetPoint );
    if ( graph[ distance(startPoint) ].neigbors.size() && graph[ distance(targetPoint) ].neigbors.size() ){
        for( int i = 0; shortest_path[i] != startPoint ; i++ ){
            int cur_point = graph[ std::distance(  material_points.begin() , material_points.find( shortest_path[i] )  )].parent;
            shortest_path.push_back( *next(material_points.begin(),cur_point) );
        }
    }
}

// True if we need to found path
bool Environment::extend_graph(){
    bool res = ! point_in_obstakle( startPoint ) && ! point_in_obstakle( targetPoint );
    if ( res ){
        // Inspect need to remove start/target points in future
        pointsWrap.removeStartPoint = material_points.find(startPoint) == material_points.end();
        pointsWrap.removeTargetPoint = material_points.find(targetPoint) == material_points.end();
        if ( pointsWrap.removeStartPoint )
            // Add point
            material_points.insert(startPoint);

        if (pointsWrap.removeTargetPoint )
            // Add point
            material_points.insert(targetPoint);

        if ( pointsWrap.removeStartPoint  || pointsWrap.removeTargetPoint   )
            triangulate();



    } else
        pointsWrap.removeStartPoint = pointsWrap.removeTargetPoint = false;
    return res;
}

void Environment::squeeze_graph(){
    if ( pointsWrap.removeStartPoint ){
        // Remove point
        material_points.remove(startPoint);
        // Remove edge
        //edges.erase( std::find( edges.begin() , edges.end() ,  pointsWrap.startEdge ) );
    }
    if ( pointsWrap.removeTargetPoint ){
        // Remove point
        material_points.remove(targetPoint);
        // Remove edge
        //edges.erase( std::find( edges.begin() , edges.end() ,  pointsWrap.targetEdge ) );
    }

}
QPoint Environment::get_nearest_point(QPoint newPoint){
    QPoint res ;
    int minLen = SCR_LEN_X;
    for ( auto point : material_points )
        if ( length(point,newPoint) < minLen ){
            minLen = length(point,newPoint) ;
            res = point;
        }
    return res;

}

int Environment::distance(QPoint point){
    return std::distance( material_points.begin() , material_points.find( point )  );
}
void Environment::mousePressEvent(QMouseEvent *ev){
    if (startPointSet){
        targetPoint = ev->pos();//get_nearest_point( ev->pos() );
        // If new edges not in obstacle
     /*   if( ! line_in_obstakle( create_material_line( startPoint , get_nearest_point(startPoint) ) ) &&
            ! line_in_obstakle( create_material_line( targetPoint , get_nearest_point(targetPoint) ) ) )
        { */
            if ( extend_graph() )  Dijkstra();
            squeeze_graph();
        //}
        startPointSet = false;
    }
    else{
        startPoint = ev->pos();//get_nearest_point( ev->pos() );
        startPointSet = true;
    }
}
