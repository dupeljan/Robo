#ifndef CALCULATION_H
#define CALCULATION_H
#include <set>
#include <utility>
#include <math.h>
#include <vector>

using namespace std;

class Calculation
{
     set< pair < int , int > > obstacle, robot , result;
     vector < pair < int , int > > polyline_points;
public:
    Calculation();
    set< pair < int , int > > get_result(){ return result; }
    set< pair < int , int > > get_obstacle(){ return obstacle; }
    set< pair < int , int > > get_robot(){ return robot; }
    vector< pair < int , int > > get_brush_obstacle() { return polyline_points; }
};

#endif // CALCULATION_H
