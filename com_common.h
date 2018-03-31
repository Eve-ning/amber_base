#ifndef COM_COMMON_H
#define COM_COMMON_H
#include <QtCore>


class cOM_Common
{
public:
    cOM_Common();

    enum class TPTypeFlag
    {
        SV_BPM_ONLY,
        SV_ONLY    ,
        BPM_ONLY   ,
        INVALID
    };

    enum class HOTypeFlag
    {
        NN_LN_ONLY,
        NN_ONLY   ,
        LN_ONLY   ,
        INVALID
    };

    enum class EHOTypeFlag
    {
        EHO_ONLY,
        INVALID
    };

    enum class inputTypeFlag
    {
        TP_ONLY,
        EHO_ONLY,
        HO_ONLY,
        MULTIPLETYPES,
        INVALID
    };

    static cOM_Common::HOTypeFlag isHO(QString HO);
    static cOM_Common::HOTypeFlag isHO(QStringList HO);

    static cOM_Common::TPTypeFlag isTP(QString TP);
    static cOM_Common::TPTypeFlag isTP(QStringList TP);

    static cOM_Common::EHOTypeFlag isEHO(QString EHO);
    static cOM_Common::EHOTypeFlag isEHO(QStringList EHO);

    static cOM_Common::inputTypeFlag isOM_Type(QString input);
    static cOM_Common::inputTypeFlag isOM_Type(QStringList input);
};

#endif // cOM_Common_H
