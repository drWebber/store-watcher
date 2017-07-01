#ifndef STOREUPDATER_H
#define STOREUPDATER_H

#include <qstring.h>
#include <qthread.h>
#include "store-remainings/storeremainings.h"

class StoreUpdater : public QThread
{
public:
    StoreUpdater(QString xlsFilePath);
    void update();
    void run();
private:
    QString xlsFilePath;
    QString csvFilePath;
    int rowCount = -1;
};

#endif // STOREUPDATER_H
