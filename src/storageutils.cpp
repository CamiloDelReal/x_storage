#include <math.h>

#include "storageutils.hpp"


QString StorageUtils::size2str(const quint64 &value)
{
    double size = value;
    double aux = size / 1024;   //Calculate KB
    QString unit;
    if(aux < 1)
    {
        unit = "B";
    }
    else {
        size = aux;
        aux /= 1024;    //Calculate MB
        if(aux < 1)
            unit = "KB";
        else {
            size = aux;
            aux /= 1024;    //Calculate GB
            if(aux < 1)
                unit = "MB";
            else {
                size = aux;
                aux /= 1024;    //Calculate TB
                if(aux < 1)
                    unit = "GB";
                else {
                    size = aux;
                    aux /= 1024;    //Calculate PB
                    if(aux < 1)
                        unit = "TB";
                    //Continue for bigest values
                }
            }
        }
    }

    size = rint(size * 100) / 100;

    return QString("%1 %2").arg(size).arg(unit);
}
