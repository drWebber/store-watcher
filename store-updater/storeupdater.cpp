#include "storeupdater.h"
#include "xls/xlsreader.h"
#include <qregularexpression.h>
#include <qdebug.h>
StoreUpdater::StoreUpdater(QString xlsFilePath)
{
    this->xlsFilePath = xlsFilePath;
    csvFilePath = xlsFilePath.replace(QRegularExpression(".xlsx?"), ".csv").replace("/", "\\");
}

StoreUpdater::StoreUpdater(QString xlsFilePath, QFileSystemWatcher &fsw, StoreRemainings &sr)
{
    this->xlsFilePath = xlsFilePath;
    csvFilePath = xlsFilePath.replace(QRegularExpression(".xlsx?"), ".csv").replace("/", "\\");
    this->fsw = &fsw;
    this->sr = &sr;
}

void StoreUpdater::update()
{
    qDebug() << "update event";
    XlsReader *xlsReader = new XlsReader();
    xlsReader->xlsToCsv(xlsFilePath, csvFilePath);
}

void StoreUpdater::run()
{
    qDebug() << "yeeeehhh, we are in thread";
    QThread::sleep(10);
    sr->updateCurrentFile();
    //удаляем все записи остатков по smid
    //читаем новый excel-файл
    //заливаем новые остатки в базу
    fsw->addPath(sr->getCurrentFilePath()); //добавляем в wather новый файл
    qDebug() << "новый файл " + sr->getCurrentFilePath() + " добавлен в вотчер";
}

