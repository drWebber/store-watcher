#ifndef STOREUPDATER_H
#define STOREUPDATER_H

#include <qstring.h>
#include <qthread.h>
#include "store-remainings/storeremainings.h"
#include <qfilesystemwatcher.h>

class StoreUpdater : public QThread
{
public:
    StoreUpdater(QFileSystemWatcher &fsw, StoreRemainings &sr);
    void update();
    void run();
private:
    void updateCsvFilePath(QString xlsFilePath);
private:
    QString xlsFilePath;
    QString csvFilePath;
    int rowCount = -1;
    QFileSystemWatcher *fsw;
    StoreRemainings *sr;
};

#endif // STOREUPDATER_H
