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
    QList<StoreRemainings *> *rems = StoreRemainings::loadRemainings();
    QList<StoreRemainings *>::iterator it;
    for (it = rems->begin(); it != rems->end(); ++it) {
        StoreRemainings *tmp = *it;
        if(tmp->getCurrentFilePath().isEmpty() || !QFile::exists(tmp->getCurrentFilePath())){
            qDebug() << "файл " + tmp->getCurrentFilePath() + " не существует или пустой!";
            tmp->updateCurrentFile();
            emit fileIsBusy(tmp->getSmid());
            StoreUpdater *su = new StoreUpdater(*fsw, *tmp);
            connect(su, SIGNAL(importError(QString)),
                    this, SLOT(on_importError(QString)));
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
        updateRemainings(path);
    }
}

void StoreWatcher::updateRemainings(QString path)
{
    StoreRemainings *tmp = getStoreRemainings(path);
        emit fileIsBusy(tmp->getSmid());
        StoreUpdater *su = new StoreUpdater(*fsw, *tmp);
        connect(su, SIGNAL(importError(QString)),
                this, SLOT(on_importError(QString)));
        connect(su, SIGNAL(updateFinished(StoreRemainings*)),
                this, SLOT(threadFinished(StoreRemainings*)), Qt::QueuedConnection);
        su->start();
}

void StoreWatcher::threadFinished(StoreRemainings* sr)
{
    emit updateFinished(sr);
}

void StoreWatcher::on_importError(QString msg)
{
    QMessageBox::critical(NULL, "Ошибка импорта", msg);
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

