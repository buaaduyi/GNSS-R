#include "gnssr.h"

ECEF danweihua(ECEF xyz){
    ECEF XYZ;
    double l = pow(pow(xyz.x,2)+pow(xyz.y,2)+pow(xyz.z,2),0.5);
    XYZ.x = xyz.x/l;
    XYZ.y = xyz.y/l;
    XYZ.z = xyz.z/l;
    return XYZ;
}
