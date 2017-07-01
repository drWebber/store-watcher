#include "storeupdater.h"
#include "xls/xlsreader.h"
#include <qregularexpression.h>
#include <qdebug.h>
StoreUpdater::StoreUpdater(QString xlsFilePath)
{
    this->xlsFilePath = xlsFilePath;
    updateCsvFilePath(xlsFilePath);
}

StoreUpdater::StoreUpdater(QString xlsFilePath, QFileSystemWatcher &fsw, StoreRemainings &sr)
{
    this->xlsFilePath = xlsFilePath;
    updateCsvFilePath(xlsFilePath);
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
    xlsFilePath = sr->getCurrentFilePath();
    updateCsvFilePath(xlsFilePath);
    update();
    //удаляем все записи остатков по smid
    //читаем новый excel-файл
    //заливаем новые остатки в базу
    fsw->addPath(xlsFilePath); //добавляем в wather новый файл
    qDebug() << "новый файл " + xlsFilePath + " добавлен в вотчер";
}

void StoreUpdater::updateCsvFilePath(QString xlsFilePath)
{
    csvFilePath = xlsFilePath.replace(QRegularExpression(".xlsx?"), ".csv").replace("/", "\\");
}

