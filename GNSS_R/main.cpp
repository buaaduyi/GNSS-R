//by duyi 2019.06.27

#include "gnssr.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GNSSR w;
    w.setWindowTitle("地基反射事件计算器");
    w.show();
    QStringList list;
    QFile file("/Users/duyi/Desktop/GPS.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

    }
    QTextStream in(&file);
    return a.exec();
}
