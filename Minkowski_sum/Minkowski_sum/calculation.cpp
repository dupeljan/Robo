#include "calculation.h"
#include <iostream>
#define PI 3.14159265358979323846

Calculation::Calculation()
{
    const int Lcorn(179), Rcorn(321);// obstacle consts

    //Calculate sqare points boundary points

    for ( int i = Lcorn; i <= Rcorn; i++){
        obstacle.insert(make_pair(Lcorn,i));
        obstacle.insert(make_pair(Rcorn,i));
        obstacle.insert(make_pair(i,Rcorn));
        obstacle.insert(make_pair(i,Lcorn));
     }

    const double alpha = PI / 6;
    int centre = (Lcorn + Rcorn) / 2;
    set< pair < int , int > > rot_obstacle;
    // Rotate points
    std::cout << cos(alpha) << '|' << sin(alpha);
    for (  set< pair < int , int > >::iterator it = obstacle.begin(); it != obstacle.end(); it++ ){
            int x = ( it->first + centre ) * cos(alpha) - ( it->second + centre ) * sin(alpha);
            int y = ( it->first + centre ) * sin(alpha) + ( it->second + centre ) * cos(alpha);
            rot_obstacle.insert(make_pair(x  ,y ));
    }
    obstacle = rot_obstacle;

    const int diam(41); // Robot const
    //Calculate cicle points

    int r = diam / 2;   // radius
    for (int x = -r ; x <= r; x++)
        for (int y = -r ; y <= r; y++ )
            if ( x * x + y * y <= r*r )
                robot.insert(make_pair(x,y));


    //Compute Minkovski sum
    for(  set< pair <int,int> >::iterator i = obstacle.begin(); i != obstacle.end(); i++)
        for( set< pair <int,int> >::iterator  j = robot.begin(); j != robot.end(); j++)
            result.insert(make_pair( i->first + j->first , i->second + j->second   ));


}
