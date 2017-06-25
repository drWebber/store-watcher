#ifndef STOREUPDATER_H
#define STOREUPDATER_H

#include <qstring.h>
#include "store-remainings/storeremainings.h"

class StoreUpdater
{
public:
    StoreUpdater(QString xlsFilePath);
    void update();
private:
    QString xlsFilePath;
    QString csvFilePath;
    int rowCount = -1;
};

#endif // STOREUPDATER_H
