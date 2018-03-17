#include <QCoreApplication>
#include <QDebug>
#include "com_map.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFileInfo mapFile;

    mapFile.setFile("D:\\osu!\\Songs\\532770 Tyrfing - Verflucht\\Tyrfing - Verflucht (sankansuki) [Cursed].osu");

    cOM_Map map;

    map.loadMap(mapFile);

    map.getMapInfo();

    return a.exec();


}
