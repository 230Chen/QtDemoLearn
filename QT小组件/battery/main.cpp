#include "dynamic_battery.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font;
    font.setFamily("microsoft yahei");
    font.setPixelSize(13);
    a.setFont(font);


    Dynamic_battery w;
    w.setWindowTitle("电池电量控件");
    w.show();
    return a.exec();
}
