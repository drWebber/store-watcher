#ifndef STOREUPDATER_H
#define STOREUPDATER_H

#include <qstring.h>
#include <qthread.h>
#include "store-remainings/storeremainings.h"
#include <qfilesystemwatcher.h>
#include <qsqlquery.h>
#include <qsqlerror.h>

class StoreUpdater : public QThread
{
    Q_OBJECT
public:
    StoreUpdater(QFileSystemWatcher &fsw, StoreRemainings &sr);
    void update();
    void run();
    QSqlDatabase getConnection();
private:
    void updateCsvFilePath(QString xlsFilePath);
signals:
    StoreRemainings *updateFinished(StoreRemainings *sr);
    QString importError(QString msg);
private:
    QString xlsFilePath;
    QString csvFilePath;
    int rowCount = -1;
    QFileSystemWatcher *fsw;
    StoreRemainings *sr;
};

#endif // STOREUPDATER_H
