#include <QCoreApplication>
#include <QDebug>
#include "com_map.h"

void calibrate();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    calibrate();


    return a.exec();


}

void calibrate(){

    QFileInfo mapFile;

    mapFile.setFile("D:\\osu!\\Songs\\165991 Reol - +Danshi\\Reol - +Danshi (lZenxl) [7K NM].osu");

    cOM_Map map;

    map.loadMap(mapFile);

    map.getInfo();

    for (int temp = 0; temp < 3; temp ++) {

        map.getOM_HOList ()[temp].getInfo();
        map.getOM_TPList ()[temp].getInfo();
        map.getBreakPList()[temp].getInfo(); // Maps may not have breaks

    }


}
