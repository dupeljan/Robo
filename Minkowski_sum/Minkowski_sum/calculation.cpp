#include "calculation.h"
#define PI 3.14159265358979323846

Calculation::Calculation()
{
    const int Lcorn(179), Rcorn(321);// Square consts

    //Calculate sqare points boundary points
    set< pair < int , int > > square;
    for ( int i = Lcorn; i <= Rcorn; i++){
        square.insert(make_pair(Lcorn,i));
        square.insert(make_pair(Rcorn,i));
        square.insert(make_pair(i,Rcorn));
        square.insert(make_pair(i,Lcorn));
     }

    const double alpha = PI / 6;
    int radius = (Lcorn + Rcorn) / 4;
    set< pair < int , int > > rot_square;
    // Rotate points
    for (  set< pair < int , int > >::iterator it = square.begin(); it != square.end(); it++ ){
            int x = ( it->first + radius ) * cos(alpha) - ( it->second + radius ) * sin(alpha);
            int y = ( it->first + radius ) * sin(alpha) + ( it->second + radius ) * cos(alpha);
            rot_square.insert(make_pair(x,y));
    }
   square = rot_square;

    const int diam(41); // Robot const
    //Calculate cicle points
    set< pair < int , int > > circle;
    int r = diam / 2;   // radius
    for (int x = -r ; x <= r; x++)
        for (int y = -r ; y <= r; y++ )
            if ( x * x + y * y <= r*r )
                circle.insert(make_pair(x,y));


    //Compute Minkovski sum
    for(  set< pair <int,int> >::iterator i = square.begin(); i != square.end(); i++)
        for( set< pair <int,int> >::iterator  j = circle.begin(); j != circle.end(); j++)
            result.insert(make_pair( i->first + j->first , i->second + j->second   ));

}
