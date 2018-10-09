#ifndef CALCULATION_H
#define CALCULATION_H
#include <set>
#include <utility>
#include <math.h>

using namespace std;

class Calculation
{
     set< pair < int , int > > obstacle, robot , result;
public:
    Calculation();
    set< pair < int , int > > get_result(){ return result; }
    set< pair < int , int > > get_obstacle(){ return obstacle; }
    set< pair < int , int > > get_robot(){ return robot; }
};

#endif // CALCULATION_H
