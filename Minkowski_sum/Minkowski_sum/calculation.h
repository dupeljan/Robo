#ifndef CALCULATION_H
#define CALCULATION_H
#include <set>
#include <utility>
#include <math.h>

using namespace std;

class Calculation
{
     set< pair < int , int > > result;
public:
    Calculation();
    set< pair < int , int > >  get_result(){ return result; }
};

#endif // CALCULATION_H
