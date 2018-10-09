#include "calculation.h"
#include <iostream>
#include <vector>
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

    /*
    for ( int i = Lcorn; i <= Rcorn; i++)
        for ( int j = Lcorn; j <= Rcorn; j++)
            obstacle.insert(make_pair(i,j));
    */
    const double alpha = PI / 6;
    int centre = (Lcorn + Rcorn) / 2;
    set< pair < int , int > > rot_obstacle;
    // Rotate points
    std::cout << cos(alpha) << '|' << sin(alpha);
    for (  set< pair < int , int > >::iterator it = obstacle.begin(); it != obstacle.end(); it++ ){
            int x = ( it->first - centre ) * cos(alpha) - ( it->second - centre ) * sin(alpha) +0.5;
            int y = ( it->first - centre ) * sin(alpha) + ( it->second - centre ) * cos(alpha) +0.5;
            x += centre;
            y += centre;
            if (  it->first == Lcorn && it->second == Lcorn  ||
                  it->first == Lcorn && it->second == Rcorn  ||
                  it->first == Rcorn && it->second == Lcorn  ||
                  it->first == Rcorn && it->second == Rcorn
                  )
                polyline_points.push_back(make_pair(x,y));
            rot_obstacle.insert(make_pair(x,y));
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

    const int shift = 100;
    set< pair < int , int > > shifted_robot;
    for( set< pair <int,int> >::iterator  it = robot.begin(); it != robot.end(); it++){
        int x = shift + it->first;
        int y = shift + it->second;
        shifted_robot.insert(make_pair(x,y));
    }
    robot = shifted_robot;
}
