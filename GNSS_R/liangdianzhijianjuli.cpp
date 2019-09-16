#include "gnssr.h"

double juli(ECEF xyz_1,ECEF xyz_2){
    double d;
    d = sqrt(pow(xyz_1.x-xyz_2.x,2)+pow(xyz_1.y-xyz_2.y, 2)+pow(xyz_1.z-xyz_2.z,2));
    return  d;
}
