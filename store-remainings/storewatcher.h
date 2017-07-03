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
private slots:
    void storeRemainsChanged(QString path);
private:
    QVector<StoreRemainings*> sr;
    QFileSystemWatcher *fsw;
};

#endif // STOREWATCHER_H
