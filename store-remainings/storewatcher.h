#ifndef STOREWATCHER_H
#define STOREWATCHER_H
#include <qstring.h>
#include <qvector.h>
#include "store-remainings/storeremainings.h"
#include <qfilesystemwatcher.h>

class StoreWatcher : public QObject
{
    Q_OBJECT
public:
    StoreWatcher();
    void setUp();
    void addFile(QString filePath);
    void removeFile(QString filePath);
    void updateRemainings(QString path);
private slots:
    void threadFinished(StoreRemainings *sr);
    void on_importError(QString msg);
    void storeRemainsChanged(QString path);
signals:
    int fileIsBusy(int smid);
    StoreRemainings *updateFinished(StoreRemainings *sr);
private:
    QVector<StoreRemainings*> sr;
    QFileSystemWatcher *fsw;
    StoreRemainings *getStoreRemainings(QString path);
};

#endif // STOREWATCHER_H
