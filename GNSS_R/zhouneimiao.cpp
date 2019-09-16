#include "gnssr.h"

QString zhouneimiaozhuanhuan(double t){

    QString s;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    while(t>=86400){
        t = t-86400;
        count1++;
    }
    while(t>=3600){
        t = t-3600;
        count2++;
    }
    while(t>=60){
        t = t-60;
        count3++;
    }
    QString day;
    switch(count1){
        case 0:
            day ="周日";
            break;
        case 1:
            day ="周一";
            break;
        case 2:
            day ="周二";
            break;
        case 3:
            day ="周三";
            break;
        case 4:
            day ="周四";
            break;
        case 5:
            day ="周五";
            break;
        case 6:
            day ="周六";
            break;
        default:
            break;

    }
    s = day+QString::number(count2)+"时"+QString::number(count3)+"分"+QString::number(int(t))+"秒";
 //   s = to_string(count2)+"时"+to_string(count3)+"分"+to_string(int(t))+"秒";
        return s;
}
