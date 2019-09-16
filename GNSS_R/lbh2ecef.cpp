#include "gnssr.h"

ECEF lbh2ecef(LBH lbh){
    ECEF xyz;
    double N = a84/(pow(1-e2*pow(sin(lbh.Lat*PI/180.0),2),0.5));
    xyz.x = (N+lbh.Hei)*cos(lbh.Lat*PI/180.0)*cos(lbh.Lon*PI/180.0);
    xyz.y = (N+lbh.Hei)*cos(lbh.Lat*PI/180.0)*sin(lbh.Lon*PI/180.0);
    xyz.z = (N*(1-e2)+lbh.Hei)*sin(lbh.Lat*PI/180.0);
    return xyz;
}
