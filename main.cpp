#include <QCoreApplication>
#include <QDebug>
#include "com_map.h"
#include "com_math.h"

void calibrate();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    calibrate();

    return a.exec();
}

void calibrate(){

    bool mapDebugBool     = false,
         TPDebugBool      = true,
         sortDebugBool    = false;

    // Map Debug
    if (mapDebugBool){

        QFileInfo mapFile;

        mapFile.setFile("D:\\osu!\\Songs\\165991 Reol - +Danshi\\Reol - +Danshi (lZenxl) [7K NM].osu");

        cOM_Map map;

        map.loadMap(mapFile);

        map.getInfo();

        for (int temp = 0; temp < 1; temp ++) {
            map.getOM_HOList ()[temp].getInfo();
            map.getOM_TPList ()[temp].getInfo();
            map.getBreakPList()[temp].getInfo(); // Maps may not have breaks
        }
    }
    if (TPDebugBool){

        cOM_TPList TPList_A_add,
                   TPList_A_subtract,
                   TPList_A_multiply,
                   TPList_A_divide,

                   TPList_B;
        /* Visualization
         *
         * Each bar represents 10000ms
         *    [1] [2] [3] [4] [5] [6] [7] [8] [9]
         * A |0.5|   |1.0|2.0|   |   |0.1|0.5|   |
         * B |   |2.0|   |5.0|4.0|2.0|   |   |0.4|
         */

        TPList_A_add      =
        TPList_A_subtract =
        TPList_A_multiply =
        TPList_A_divide   = QStringList({
            "10000,-200,4,2,1,80,0,0" , // [1]
            "30000,-100,4,2,1,80,0,0" , // [3]
            "40000,-50,4,2,1,80,0,0"  , // [4]
            "70000,-1000,4,2,1,80,0,0", // [7]
            "80000,-200,4,2,1,80,0,0"   // [8]
                   });

        TPList_B = QStringList({
            "20000,-50,4,2,1,80,0,0"  , // [2]
            "40000,-20,4,2,1,80,0,0"  , // [4]
            "50000,-25,4,2,1,80,0,0"  , // [5]
            "60000,-50,4,2,1,80,0,0"  , // [6]
            "90000,-250,4,2,1,80,0,0"   // [9]
                   });

        TPList_A_add     .add     (TPList_B);
        TPList_A_subtract.subtract(TPList_B);
        TPList_A_multiply.multiply(TPList_B);
        TPList_A_divide  .divide  (TPList_B);

        qDebug() << "[--- ADD ---]";
        qDebug() << "          " << TPList_A_add.getValueList();
        qDebug() << "Expected : (0.5, 3, 7, 2.1, 2.5)";

        qDebug() << "[--- subtract ---]";
        qDebug() << "          " << TPList_A_subtract.getValueList();
        qDebug() << "Expected : (0.5, -1, -3, -1.9, -1.5)";

        qDebug() << "[--- MULTIPLY ---]";
        qDebug() << "          " << TPList_A_multiply.getValueList();
        qDebug() << "Expected : (0.5, 2, 10, 0.2, 1)";

        qDebug() << "[--- DIVIDE ---]";
        qDebug() << "          " << TPList_A_divide.getValueList();
        qDebug() << "Expected : (0.5, 0.5, 0.4, 0.05, 0.25)";

    }
    if (sortDebugBool){
        cOM_HOList HOList ("00:00:257 (299|4,291|5,3432|6,38025|4,4128|5,557|4) - ", 7);

        qDebug() << "Original   : " << HOList.getOffsetList() << HOList.getColumnList();
        HOList.sortOffset(true);
        qDebug() << "Ascending  : " << HOList.getOffsetList() << HOList.getColumnList();
        HOList.sortOffset(false);
        qDebug() << "Descending : " << HOList.getOffsetList() << HOList.getColumnList();
    }




}
