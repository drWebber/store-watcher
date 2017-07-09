#include "store-remainings/storewatcher.h"
#include "store-updater/storeupdater.h"
#include <qsqlquery.h>
#include <qmessagebox.h>
#include <qthread.h>
#include <qdebug.h>

StoreWatcher::StoreWatcher()
{
    fsw = new QFileSystemWatcher();
    connect(fsw, SIGNAL(fileChanged(QString)), this, SLOT(storeRemainsChanged(QString)));
}

void StoreWatcher::setUp()
{
    QSqlQuery query;
    query.exec("SELECT * FROM store_manufacturer");
    while (query.next()) {
        int smid = query.value("smid").toInt();
        int mid = query.value("mid").toInt();
        QString path = query.value("path").toString();
        QString regExp = query.value("regexp").toString();
        QString lastFilePath = query.value("lastPath").toString();
        int startRow = query.value("startRow").toInt();
        int articleCol = query.value("articleCol").toInt();
        int itemCountCol = query.value("itemCountCol").toInt();

        StoreRemainings *tmp = new StoreRemainings(smid, mid, path, regExp, lastFilePath, startRow,
                                                   articleCol, itemCountCol);

        if(lastFilePath.isEmpty() || !QFile::exists(lastFilePath)){
            qDebug() << "файл " + lastFilePath + " не существует или пустой!";
            tmp->updateCurrentFile();
            emit fileIsBusy(tmp->getSmid());
            StoreUpdater *su = new StoreUpdater(*fsw, *tmp);
            su->start();
            connect(su, SIGNAL(updateFinished(StoreRemainings*)), this, SLOT(threadFinished(StoreRemainings*)), Qt::QueuedConnection);
        } else {
            fsw->addPath(tmp->getCurrentFilePath());
        }

        sr.append(tmp);
    }
}

void StoreWatcher::addFile(QString filePath)
{
    qDebug() << "файл " + filePath + " добавлен в watcher";
    fsw->addPath(filePath);
}

void StoreWatcher::removeFile(QString filePath)
{
    qDebug() << "файл " + filePath + " удален из watcher'a";
    fsw->removePath(filePath);
}

void StoreWatcher::storeRemainsChanged(QString path)
{
    if(!QFile::exists(path)) {
        qDebug() << "файл " + path + " недоступен";
        StoreRemainings *tmp = getStoreRemainings(path);
        emit fileIsBusy(tmp->getSmid());
        StoreUpdater *su = new StoreUpdater(*fsw, *tmp);
        su->start();
        connect(su, SIGNAL(updateFinished(StoreRemainings*)), this, SLOT(threadFinished(StoreRemainings*)), Qt::QueuedConnection);
    }
}

void StoreWatcher::threadFinished(StoreRemainings* sr)
{
    emit updateFinished(sr);
}

StoreRemainings *StoreWatcher::getStoreRemainings(QString path)
{
    StoreRemainings *tmp;
    for (int i = 0; i < sr.count(); ++i) {
        tmp = sr.at(i);
        QString tmpPath = tmp->getCurrentFilePath();
        if (tmpPath.contains(path)) {
            fsw->removePath(tmpPath); //удаляем из wather'a исчезнувший файл
            break;
        }
    }
    return tmp;
}

