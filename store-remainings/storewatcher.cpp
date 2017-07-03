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
    this->setUp();
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

        if(lastFilePath.isEmpty()){
            tmp->updateCurrentFile();
            StoreUpdater su(*tmp);
            su.update();
            qDebug() << tmp->getCurrentFilePath();
        }

        if(QFile::exists(tmp->getCurrentFilePath())){
            sr.append(tmp);
            fsw->addPath(tmp->getCurrentFilePath());
        } else {
            QMessageBox::information(NULL, "Ошибка", "Файл " + tmp->getCurrentFilePath()
                                     + " не найден! smid = " + mid);
        }
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
    qDebug() << "файл " + path + " удален";
    for (int i = 0; i < sr.count(); ++i) {
        StoreRemainings *tmp = sr.at(i);
        QString tmpPath = tmp->getCurrentFilePath();
        if (tmpPath.contains(path)) {
            fsw->removePath(tmpPath); //удаляем из wather'a исчезнувший файл
            //создаем StoreUpdater как отдельный поток
            StoreUpdater *su = new StoreUpdater(tmp->getCurrentFilePath(), *fsw, *tmp);
            su->start();
        }
    }
}

