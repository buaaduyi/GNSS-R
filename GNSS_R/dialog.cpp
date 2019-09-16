#include "dialog.h"
#include "ui_dialog.h"
#include "gnssr.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

extern QVector<LBH> worldmap;
extern LBH center_point;

void Dialog::paintEvent(QPaintEvent *){
    int n =worldmap.size();
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    QPointF point[n];
    for(int i=0;i<n;i++){
        point[i].setX(worldmap[i].Lon*3+580);
        point[i].setY(-worldmap[i].Lat*3+350);
    }
    painter.drawPoints(point,n);

    pen.setColor(Qt::red);
    pen.setWidth(4);
    painter.setPen(pen);
    QPointF centerpoint[1];
    centerpoint[0].setX(center_point.Lon*3+580);
    centerpoint[0].setY(-center_point.Lat*3+350);
    painter.drawPoints(centerpoint,1);

}
