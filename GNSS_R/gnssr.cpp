//by DUYI 2019.06.27

#include "gnssr.h"
#include "ui_gnssr.h"

GNSSR::GNSSR(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GNSSR)
{
    ui->setupUi(this);

}
extern QStringList list;
extern int num;
GNSSR::~GNSSR()
{
    delete ui;
}

QVector<LBH> worldmap{};
LBH center_point;

void GNSSR::on_run_Button_clicked()
{
    ui->list1->clear();
//    double Lat = 39.978551; //接收机纬度
//    double Lon = 116.344514; //接收机经度
//    double Hei = 98.34; //接收机高度
//    double phi = 50; //接收机天线半功率波束宽度(HPBW)
//    double alpha = 50; //接收机天线俯仰角 俯仰角不能太小
//    double beta =90; //接收机天线方位角（正北为0°，顺时针增大）
    double Lat = ui->lineEdit1->text().toDouble();
    double Lon = ui->lineEdit2->text().toDouble();
    double Hei = ui->lineEdit3->text().toDouble();
    double phi = ui->lineEdit6->text().toDouble();
    double alpha = ui->lineEdit5->text().toDouble();
    double beta = ui->lineEdit4->text().toDouble();
    double freq = 1/(ui->lineEdit7->text().toDouble());
    if(Lat==0.0||Lon==0.0||Hei==0.0||phi==0.0||alpha==0.0||beta==0.0){

        ui->list1->addItem("请输入参数");
        return;
    }
    if(fileName==""){
        ui->list1->addItem("请导入星历文件");
        return;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->list1->addItem("error");
        return;
    }
    QTextStream in(&file);

    QFile output1("/Users/duyi/Desktop/lat.txt");
    if (!output1.open(QFile::WriteOnly|QFile::Truncate)){
        ui->list1->addItem("error");
        return;
    }
    QTextStream out1(&output1);

    QFile output2("/Users/duyi/Desktop/lon.txt");
    if (!output2.open(QFile::WriteOnly|QFile::Truncate)){
        ui->list1->addItem("error");
        return;
    }
    QTextStream out2(&output2);

    QString prn;
    int counts = 0;
    while(!file.atEnd()){
        QString s =file.readLine();
        if(counts == 0){
                prn = s.mid(0,3);
                }
                if(counts<7){
                    QString s1 = s.mid(4,19);
                    QString s2 = s.mid(23,19);
                    QString s3 = s.mid(42,19);
                    QString s4 = s.mid(61,19);

                    double b1=s1.toDouble();
                    double b2=s2.toDouble();
                    double b3=s3.toDouble();
                    double b4=s4.toDouble();
                    EPH_par.push_back(b1);
                    EPH_par.push_back(b2);
                    EPH_par.push_back(b3);
                    EPH_par.push_back(b4);
                }

                else{
                    eph_par par;
                    par.prn = prn;
                    par.toe = EPH_par[12];
                    par.root_as = EPH_par[11];
                    par.es = EPH_par[9];
                    par.i0 = EPH_par[16];
                    par.omega_0 = EPH_par[14];
                    par.w = EPH_par[18];
                    par.M0 = EPH_par[7];
                    par.deta_n = EPH_par[6];
                    par.dot_i = EPH_par[20];
                    par.dot_omega = EPH_par[19];
                    par.Cuc = EPH_par[8];
                    par.Cus = EPH_par[10];
                    par.Crc = EPH_par[17];
                    par.Crs = EPH_par[5];
                    par.Cic = EPH_par[13];
                    par.Cis = EPH_par[15];
                    par.af1 = EPH_par[2];
                    par.af2 = EPH_par[3];
                    eph.push_back(par);
                    EPH_par.clear();
                    counts = -1;
                }
                counts++;
    }

    LBH lbh_r; //接收机经纬高
        LBH lbh_r_mirror; //接收机关于地面的对称点
        LBH lbh_r_ground; //接收机在地面上的投影点
        LBH lbh_north;
        LBH lbh_south;
        LBH lbh_east;
        LBH lbh_west;

        ECEF xyz_r; //接收机ECEF
        ECEF xyz_r_mirror; //接收机关于地面的对称点
        ECEF xyz_r_ground; //接收机在地面上的投影点ECEF
        ECEF xyz_north;
        ECEF xyz_south;
        ECEF xyz_east;
        ECEF xyz_west;
        ECEF xyz_vector_up;
        ECEF xyz_vector_north;
        ECEF xyz_vector_east;
        ECEF xyz_vector_r; //最近点到最远点的方向向量
        ECEF xyz_vector_center;
        ECEF xyz_short; //椭圆最近点
        ECEF xyz_long;  //椭圆最远点
        ECEF xyz_center; //椭圆中心

        lbh_r.Lat = Lat;
        lbh_r.Lon = Lon;
        lbh_r.Hei = Hei;
        lbh_r_ground.Lon = lbh_r.Lon;
        lbh_r_ground.Lat = lbh_r.Lat;
        lbh_r_ground.Hei = 0.0;

        lbh_r_mirror.Lon = lbh_r.Lon;
        lbh_r_mirror.Lat = lbh_r.Lat;
        lbh_r_mirror.Hei = -lbh_r.Hei;

        lbh_north.Lat = lbh_r.Lat+0.0001; //北
        lbh_north.Lon = lbh_r.Lon;
        lbh_north.Hei = 0.0;

        lbh_south.Lat = lbh_r.Lat-0.0001; //南
        lbh_south.Lon = lbh_r.Lon;
        lbh_south.Hei = 0.0;

        lbh_east.Lat = lbh_r.Lat;
        lbh_east.Lon = lbh_r.Lon+0.0001;  //东
        lbh_east.Hei = 0.0;

        lbh_west.Lat = lbh_r.Lat;
        lbh_west.Lon = lbh_r.Lon-0.0001;  //西
        lbh_west.Hei = 0.0;

        xyz_r = lbh2ecef(lbh_r);
        xyz_r_ground = lbh2ecef(lbh_r_ground);
        xyz_north = lbh2ecef(lbh_north);
        xyz_south = lbh2ecef(lbh_south);
        xyz_east = lbh2ecef(lbh_east);
        xyz_west = lbh2ecef(lbh_west);
        xyz_r_mirror = lbh2ecef(lbh_r_mirror);

        double A=0.0; //探测区椭圆长轴
            double B=0.0; //探测区椭圆短轴

            double L1=0.0; //短边界
            double L2=0.0; //长边界
            if(alpha>phi/2){
                if((alpha+phi/2)<=90)
                {
                    L1 = Hei*tan((90-alpha-phi/2)/180*PI);
                    L2 = Hei*tan((90-alpha+phi/2)/180*PI);
                    A = L2-L1;   //探测区椭圆长轴
                    B = pow(pow((L1+L2)/2,2)+pow(Hei,2),0.5)*tan((phi/2)/180*PI); //探测区椭圆短轴
                }

                else{
                    L1 = Hei*tan((alpha+phi/2-90)/180*PI);
                    L2 = Hei*tan((90-alpha+phi/2)/180*PI);
                    A = L2+L1;   //探测区椭圆长轴
                    B = 2*pow(pow((L1+L2)/2-L1,2)+pow(Hei,2),0.5)*tan((phi/2)/180*PI); //探测区椭圆短轴
                }
            }
            else{
                ui->list1->addItem("接收机天线俯仰角过小");
                return;
            }

            //接收机正上方向向量
                xyz_vector_up = xyz_r-xyz_r_ground;
                xyz_vector_up = danweihua(xyz_vector_up);
                //接收机正北方向向量
                xyz_vector_north = xyz_north-xyz_south;
                xyz_vector_north = danweihua(xyz_vector_north);
                //接收机正东方向向量
                xyz_vector_east = xyz_east-xyz_west;
                xyz_vector_east = danweihua(xyz_vector_east);


                //计算接收机方位朝向的方向向量
                if(beta>=0&&beta<360){

                    xyz_vector_r.x=xyz_vector_north.x*cos(beta*PI/180)+xyz_vector_east.x*sin(beta*PI/180);
                    xyz_vector_r.y=xyz_vector_north.y*cos(beta*PI/180)+xyz_vector_east.y*sin(beta*PI/180);
                    xyz_vector_r.z=xyz_vector_north.z*cos(beta*PI/180)+xyz_vector_east.z*sin(beta*PI/180);


                }
                else{

                    ui->list1->addItem("方位角的范围是0°~360°");
                    return;

                }

                xyz_vector_r = danweihua(xyz_vector_r); //单位化方向向量

                if(alpha>phi/2){
                    if((alpha+phi/2)<=90)
                    {
                        xyz_short.x = xyz_r_ground.x+xyz_vector_r.x*L1;
                        xyz_short.y = xyz_r_ground.y+xyz_vector_r.y*L1;
                        xyz_short.z = xyz_r_ground.z+xyz_vector_r.z*L1;

                        xyz_long.x = xyz_r_ground.x+xyz_vector_r.x*L2;
                        xyz_long.y = xyz_r_ground.y+xyz_vector_r.y*L2;
                        xyz_long.z = xyz_r_ground.z+xyz_vector_r.z*L2;
                    }
                    else{
                        xyz_short.x = xyz_r_ground.x-xyz_vector_r.x*L1;
                        xyz_short.y = xyz_r_ground.y-xyz_vector_r.y*L1;
                        xyz_short.z = xyz_r_ground.z-xyz_vector_r.z*L1;

                        xyz_long.x = xyz_r_ground.x+xyz_vector_r.x*L2;
                        xyz_long.y = xyz_r_ground.y+xyz_vector_r.y*L2;
                        xyz_long.z = xyz_r_ground.z+xyz_vector_r.z*L2;
                    }
                }
                else{
                    ui->list1->addItem("接收机天线俯仰角过小");
                    return;
                }

                double juli1 = juli(xyz_short, xyz_r);  //短
                double juli2 = juli(xyz_long, xyz_r);   //长


                double l1 = (A*Hei/juli1)/(Hei/juli1+Hei/juli2);

                xyz_center.x = xyz_long.x-l1*xyz_vector_r.x;
                xyz_center.y = xyz_long.y-l1*xyz_vector_r.y;
                xyz_center.z = xyz_long.z-l1*xyz_vector_r.z;



                xyz_vector_center.x = xyz_center.x-xyz_r_mirror.x;
                xyz_vector_center.y = xyz_center.y-xyz_r_mirror.y;
                xyz_vector_center.z = xyz_center.z-xyz_r_mirror.z;
                xyz_vector_center = danweihua(xyz_vector_center);
                center_point = ecef2lbh((xyz_center));

    for(auto c:eph){

                        QVector<TANCEQU> tancequ;
                        int count1=0;
                        if(c.prn[0]=='G'){
                            for(int i=-7200;i<=0;i=i+int(freq)){

                                ECEF pos = cal_pos(i, c);
                           //     cout<<pos<<endl;
                                ECEF xyz_vector_sat = pos-xyz_r_mirror;

                                double jiajiaoyuxian = (xyz_vector_sat.x*xyz_vector_center.x+xyz_vector_sat.y*xyz_vector_center.y+xyz_vector_sat.z*xyz_vector_center.z)/(sqrt(pow(xyz_vector_sat.x,2)+pow(xyz_vector_sat.y,2)+pow(xyz_vector_sat.z,2))*sqrt(pow(xyz_vector_center.x,2)+pow(xyz_vector_center.y,2)+pow(xyz_vector_center.z,2)));

                                if(jiajiaoyuxian>0){
                                    double jiajiao = acos(jiajiaoyuxian)*180/PI;
                              //      cout<<jiajiao<<endl;
                                    if(jiajiao<(phi)/2){
                                    //    cout<<jiajiao<<endl;
                                    //cout<<"第"<<i<<"秒卫星可见"<<endl;
                                        kejianqujian.push_back(i);
                                        TANCEQU tance;
                                        tance = cal_tancequ(xyz_r_mirror,pos,Hei);
                                        tancequ.push_back(tance);
                                        count1++;
                                    }
                                }

                            }
                        }
                        if(c.prn[0]=='C'&&c.prn!="C01"&&c.prn!="C02"&&c.prn!="C03"&&c.prn!="C04"&&c.prn!="C05"){
                            for(int i=0;i<=3600;i=i+int(freq)){

                                ECEF pos = cal_pos(i, c);
                                 //   cout<<pos<<endl;
                                ECEF xyz_vector_sat = pos-xyz_r_mirror;

                                double jiajiaoyuxian = (xyz_vector_sat.x*xyz_vector_center.x+xyz_vector_sat.y*xyz_vector_center.y+xyz_vector_sat.z*xyz_vector_center.z)/(sqrt(pow(xyz_vector_sat.x,2)+pow(xyz_vector_sat.y,2)+pow(xyz_vector_sat.z,2))*sqrt(pow(xyz_vector_center.x,2)+pow(xyz_vector_center.y,2)+pow(xyz_vector_center.z,2)));

                                if(jiajiaoyuxian>0){
                                    double jiajiao = acos(jiajiaoyuxian)*180/PI;
                                 //   cout<<jiajiao<<endl;
                                    if(jiajiao<(phi)/2){

                                        kejianqujian.push_back(i);
                                        TANCEQU tance;
                                        tance = cal_tancequ(xyz_r_mirror,pos,Hei);
                                        tancequ.push_back(tance);
                                        count1++;
                                    }
                                }

                            }

                        }
                        if(count1==0){
                        //    cout<<c.prn<<"号卫星在该时段内不可见"<<endl;
                        }
                        else{


                            double begin = kejianqujian[0]+c.toe;
                            double end = kejianqujian[kejianqujian.size()-1]+c.toe;

                            QString BEG = zhouneimiaozhuanhuan(begin);
                            QString END = zhouneimiaozhuanhuan(end);
                            ui->list1->addItem("星号:"+c.prn);
                            ui->list1->addItem(BEG+"--"+END);
                            for(auto c:tancequ){
                                LBH lbh = ecef2lbh(c.specular_point);
                                out1 << QString::number(lbh.Lat,'f',8) << endl;
                                out2 << QString::number(lbh.Lon,'f',8) << endl;
                                ui->list1->addItem("纬度:"+QString::number(lbh.Lat,'f',8)+"   "+'\t'+"经度:"+QString::number(lbh.Lon,'f',8)+"   "+'\t'+"反射区长轴:"+QString::number(c.a,'f',3)+"    "+'\t'+"反射区短轴:"+QString::number(c.b,'f',3));
                            }

                        }

                        kejianqujian.clear();

                    }


}

void GNSSR::on_data_Button_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,tr("选择文件"),"/Users/duyi/Desktop/",tr("文本文件(*txt)"));

}

void GNSSR::on_clear_Button_clicked()
{
    ui->list1->clear();
    ui->lineEdit1->clear();
    ui->lineEdit2->clear();
    ui->lineEdit3->clear();
    ui->lineEdit4->clear();
    ui->lineEdit5->clear();
    ui->lineEdit6->clear();
    ui->lineEdit7->clear();
    fileName.clear();
    EPH_par.clear();
    eph.clear();
    kejianqujian.clear();
    worldmap.clear();


}

void GNSSR::on_picture_Button_clicked()
{
//    QPixmap pixmap("/Users/duyi/Desktop/build-GNSS_R-unknown-Debug/1.jpg");
//    QPixmap fitpixmap=pixmap.scaled(640,390,Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    ui->labe_p->setPixmap(fitpixmap);
//    ui->labe_p->show();

    mydialog = new Dialog(this);
    mydialog->show();
    mydialog->setGeometry(300,200,1200,700);

 //   QDir dir;
    QString filedir = "/Users/duyi/Desktop/build-GNSS_R-unknown-Debug/jingmian.app";
    process = new QProcess(this);
    process->start(filedir);

    QFile file1("/Users/duyi/Desktop/build-GNSS_R-unknown-Debug/worldlat.txt");
    QFile file2("/Users/duyi/Desktop/build-GNSS_R-unknown-Debug/worldlon.txt");
    if(!file1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    while(!file1.atEnd()&&!file2.atEnd()){
        QString s1 =file1.readLine();
        QString s2 =file2.readLine();
        LBH point;
        point.Lat = s1.toDouble();
        point.Lon = s2.toDouble();
        worldmap.push_back(point);
    }



}

void GNSSR::on_sql_Button_clicked()
{
    QStringList drivers = QSqlDatabase::drivers();
    foreach(QString driver,drivers){
        qDebug()<<driver;
    }
    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
//    database.setHostName("localhost");
//    database.setPort(3306);
//    database.setUserName("root");
//    database.setPassword("19960115");
//    database.setDatabaseName("/Users/duyi/Desktop/sql.db");

//    if(!database.open()){
//        QMessageBox::warning(this,"database error",database.lastError().text());
//        return;
//    }
//    else{
//        qDebug()<<"open success!";

//        QSqlQuery sql_query;

//        QString create_sql = "create table student (id int primary key, name varchar(30), age int)";
//        QString select_max_sql = "select max(id) from student";
//        QString insert_sql1 = "insert into student values (1,'杜毅',23)";
//        QString update_sql = "update student set name = :name where id = :id";
//        QString select_sql = "select id, name from student";
//        QString select_all_sql = "select * from student";
//        QString delete_sql = "delete from student where id = 'SY1802305'";
//        QString clear_sql = "delete from student";

     //   sql_query.prepare(create_sql);
      //  sql_query.exec(insert_sql1);
//        if(!sql_query.exec(insert_sql1))
//        {
//        qDebug()<<sql_query.lastError();
//        }
//        else
//        {
//        qDebug()<<"write success!";
//        }
     //
//        if(!sql_query.exec(clear_sql))
//        {
//        qDebug()<<sql_query.lastError();
//        }
//        else
//        {
//        qDebug()<<"delete success!";
//        }
//    }






}
