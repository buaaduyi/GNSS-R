#include "gnssr.h"


LBH ecef2lbh(ECEF xyz)
{
    LBH lbh;
    lbh.Lon=atan(xyz.y/xyz.x);
    if(lbh.Lon<0){
        lbh.Lon = lbh.Lon+PI;
    }
    double p =sqrt(xyz.x*xyz.x+xyz.y*xyz.y);
    double N=0;
    for(int i=0;i<5;i++){
        N=a84/sqrt(1-e2*pow(sin(lbh.Lat),2));
        lbh.Hei=p/cos(lbh.Lat)-N;
        lbh.Lat=atan((xyz.z/p)*pow((1-e2*N/(N+lbh.Hei)),-1));
    }
    lbh.Lat = lbh.Lat/PI*180;
    lbh.Lon = lbh.Lon/PI*180;
    return lbh;
}
