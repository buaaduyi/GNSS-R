#include <gnssr.h>


ECEF cal_pos(double t,eph_par par){

    double tk = t;
    double toe = par.toe;
    double root_as = par.root_as;
    double es = par.es;
    double i0 = par.i0;
    double omega_0 = par.omega_0;
    double w = par.w;
    double M0 = par.M0;
    double deta_n = par.deta_n;
    double dot_i = par.dot_i;
    double dot_omega = par.dot_omega;
    double Cuc = par.Cuc;
    double Cus = par.Cus;
    double Crc = par.Crc;
    double Crs = par.Crs;
    double Cic = par.Cic;
    double Cis = par.Cis;
//  double af1 = par.af1;
//  double af2 = par.af2;


    double n0 = pow(u,0.5)/pow(root_as,3);
    double n = n0+deta_n;
    double Mk = M0+n*tk;

    if(Mk>2*PI){
        Mk = Mk-2*PI;
    }
    else if(Mk<0){
        Mk = Mk+2*PI;
    }

    double Ej = Mk+es*sin(Mk);
    Ej = Mk+es*sin(Ej);
    Ej = Mk+es*sin(Ej);
    Ej = Mk+es*sin(Ej);

    double vk = atan((pow(1-es*es, 0.5)*sin(Ej))/(cos(Ej)-es));
    double cosv = (cos(Ej)-es)/(1-es*cos(Ej));
    double sinv = (pow(1-es*es, 0.5)*sin(Ej))/(1-es*cos(Ej));

    if((cosv<0)&&(sinv<0)&&(vk>0)){

        vk=vk-PI;
    }
    else if((cosv>0)&&(sinv<0)&&(vk>0)){
        vk = vk-PI;
    }

    double faik = vk+w;

    double deta_uk = Cus*sin(2*faik)+Cuc*cos(2*faik);
    double deta_rk = Crs*sin(2*faik)+Crc*cos(2*faik);
    double deta_ik = Cis*sin(2*faik)+Cic*cos(2*faik);

    double uk = faik+deta_uk;
    double rk = pow(root_as, 2)*(1-es*cos(Ej))+deta_rk;
    double ik = i0+dot_i*tk+deta_ik;

    double xk1 = rk*cos(uk);
    double yk1 = rk*sin(uk);

    double omega_k = omega_0+(dot_omega-dot_omega_e)*tk-dot_omega_e*toe;

    double xk = xk1*cos(omega_k)-yk1*cos(ik)*sin(omega_k);
    double yk = xk1*sin(omega_k)+yk1*cos(ik)*cos(omega_k);
    double zk = yk1*sin(ik);

    ECEF pos;
    pos.x = xk;
    pos.y = yk;
    pos.z = zk;

    return pos;

}
