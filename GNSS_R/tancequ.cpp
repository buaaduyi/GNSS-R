#include "gnssr.h"

TANCEQU cal_tancequ(ECEF xyz_r_mirror,ECEF xyz_sat,double h){

    TANCEQU tancequ;
    ECEF xyz_m;
    ECEF XYZ_R_MIRROR = xyz_r_mirror;

    double delta=0.0;
    do{
        xyz_m = xyz_r_mirror+xyz_sat;
        xyz_m.x = xyz_m.x/2;
        xyz_m.y = xyz_m.y/2;
        xyz_m.z = xyz_m.z/2;
        LBH lbh_m = ecef2lbh(xyz_m);
        delta = lbh_m.Hei;
        if(delta>0){
            xyz_sat = xyz_m;
        }
        else{
            xyz_r_mirror = xyz_m;
        }
    }while(fabs(delta)>0.01);

    double d_rm = juli(xyz_m, XYZ_R_MIRROR);
    double gama = asin(h/d_rm);
    double wavelength = 0.19029367;
    double a = sqrt(wavelength*h*sin(gama))/pow(sin(gama),2);  //菲涅尔反射区长半轴
    double b = sqrt(wavelength*h*sin(gama))/sin(gama);         //菲涅尔反射区短半轴

    tancequ.specular_point = xyz_m;
    tancequ.a = a;
    tancequ.b = b;

    return tancequ;
}
