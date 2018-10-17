#include "crspline.h"
#include <QPainter>
#include <QTime>
#include <QPolygon>
#include <iostream>
#include <utility>
#include <math.h>
#include <algorithm>
#include "matrixproc.h"

CRSpline::CRSpline( QPoint firstSpline , QPoint secondSpline ){
    derPoints.PFirst = firstSpline;
    derPoints.PLast = secondSpline;
}

void CRSpline::createCatmullRomSpline(QVector < QPoint > materialPoints){
    //First spline
   const std::vector < std::vector < double > > CFirst={ { 0 , 2 , 0 , 0 } ,
                                                         { 2 , 0 , 0 , 0 } ,
                                                         {-4 ,-5 , 6 ,-1 } ,
                                                         { 2 , 3 ,-4 , 1 } };

   const std::vector < std::vector < double > > CLast={ { 0 , 2 , 0 , 0 } ,
                                                        {-1 , 0 , 1 , 0 } ,
                                                        { 2 ,-6 , 4 ,-2 } ,
                                                        {-1 , 4 ,-3 , 2 } };

   const std::vector < std::vector < double > > C={ { 0 , 2 , 0 , 0 } ,
                                                    {-1 , 0 , 1 , 0 } ,
                                                    { 2 ,-5 , 4 ,-1 } ,
                                                    {-1 , 3 ,-3 , 1 } };

    std::vector < double > Px, Py, Cx, Cy;
    int n = materialPoints.size() - 1;
    for ( int i = 0; i < n  ; i++){

        if ( i == 0 ) {
            Px = { derPoints.PFirst.x() , materialPoints[i].x() , materialPoints[i+1].x() , materialPoints[i+2].x() };
            Py = { derPoints.PFirst.y() , materialPoints[i].y() , materialPoints[i+1].y() , materialPoints[i+2].y() };
            Cx = composition<double>(CFirst,Px);
            Cy = composition<double>(CFirst,Py);
       } else
        if ( i == n - 1 ) {
            Px = { materialPoints[i-1].x() , materialPoints[i].x() , materialPoints[i+1].x() , derPoints.PLast.x() };
            Py = { materialPoints[i-1].y() , materialPoints[i].y() , materialPoints[i+1].y() , derPoints.PLast.y() };
            Cx = composition<double>(CLast,Px);
            Cy = composition<double>(CLast,Py);
        } else {
            Px = { materialPoints[i-1].x() , materialPoints[i].x() , materialPoints[i+1].x() , materialPoints[i+2].x() };
            Py = { materialPoints[i-1].y() , materialPoints[i].y() , materialPoints[i+1].y() , materialPoints[i+2].y() };
            Cx = composition<double>(C,Px);
            Cy = composition<double>(C,Py);
        }
         for ( double t = 0; t <= 1; t += DELTA_T ){
            std::vector<double> T = { 1 , t , t*t , t*t*t };
            spline.push_back(QPoint( composition<double>(T,Cx) / 2.0 , composition<double>(T,Cy) / 2.0));
        }
    }
}

QVector < QPoint > CRSpline::get_spline(){
    return spline;
}
