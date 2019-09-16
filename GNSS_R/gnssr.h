#ifndef GNSSR_H
#define GNSSR_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include "qmath.h"
#include <QFileDialog>
#include <QPainter>
#include <QTextStream>
#include <QProcess>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include "dialog.h"

namespace Ui {
class GNSSR;
}

struct ECEF{
    double x;
    double y;
    double z;

    ECEF operator + (const ECEF& xyz){
        ECEF XYZ;
        XYZ.x = this->x+xyz.x;
        XYZ.y = this->y+xyz.y;
        XYZ.z = this->z+xyz.z;
        return XYZ;
    }
    ECEF operator - (const ECEF& xyz){
        ECEF XYZ;
        XYZ.x = this->x-xyz.x;
        XYZ.y = this->y-xyz.y;
        XYZ.z = this->z-xyz.z;
        return XYZ;
    }


};

struct LBH{
    double Lat; //纬度
    double Lon; //经度
    double Hei; //高度

};

struct eph_par{

    QString prn;
    double toe;
    double root_as;
    double es;
    double i0;
    double omega_0;
    double w;
    double M0;
    double deta_n;
    double dot_i;
    double dot_omega;
    double Cuc;
    double Cus;
    double Crc;
    double Crs;
    double Cic;
    double Cis;
    double af1;
    double af2;

};

struct TANCEQU{
    ECEF specular_point;
    double a;
    double b;
};

const double L1 = 1575.42e6; //GPS L1频率
const double a84 = 6378137.0; //长半径a
const double b84 = 6356752.3142; //短半径b
const double e2 = 0.00669437999014; //e为椭球偏心率
const double c = 299792458.0; //光速
const double PI = 3.1415926535898; //π
const double u = 3.986005e14;  //地球引力常数
const double dot_omega_e = 7.2921151467e-5;  //地球自转角速度
const double F = -4.442807633e-10;

extern QVector<LBH> worldmap;
extern LBH center_point;

class GNSSR : public QMainWindow
{
    Q_OBJECT

public:
    explicit GNSSR(QWidget *parent = nullptr);
    ~GNSSR();
    int num=10;
    QVector<double> EPH_par;
    QVector<eph_par> eph;
    QVector<double> kejianqujian;
    QString fileName;




private slots:
    void on_run_Button_clicked();

    void on_data_Button_clicked();

    void on_clear_Button_clicked();

    void on_picture_Button_clicked();

    void on_sql_Button_clicked();

private:
    Ui::GNSSR *ui;
    Dialog *mydialog;
    QProcess *process;
};

ECEF lbh2ecef(LBH lbh);
LBH ecef2lbh(ECEF xyz);
ECEF danweihua(ECEF xyz);
double juli(ECEF xyz_1,ECEF xyz_2);
ECEF cal_pos(double t,eph_par par);
QString zhouneimiaozhuanhuan(double t);
TANCEQU cal_tancequ(ECEF xyz_r,ECEF xyz_sat,double h);

#endif // GNSSR_H
