#ifndef COM_MAPSET_H
#define COM_MAPSET_H

#include "com_map.h"

class cOM_MapSet
{
public:
    cOM_MapSet();

private:
    QList<cOM_Map> OM_MapList;
};

#endif // COM_MAPSET_H
