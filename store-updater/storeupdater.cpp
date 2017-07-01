#include "storeupdater.h"
#include "xls/xlsreader.h"
#include <qregularexpression.h>
#include <qdebug.h>
#include <qsqlquery.h>
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

    //читаем csv
    QSqlQuery statement;
    statement.exec("START TRANSACTION");
    statement.prepare("INSERT INTO test(article, qty) VALUES (:article, :qty)");
    QFile csvFile(csvFilePath);
    if(csvFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&csvFile);
        for (int i(0); i < sr->getStartRow()-1; i++) stream.readLine(); //пропускаем нужное число строк
                                                                        //начало задано в бд остатков
        while(!stream.atEnd()){
           QString line = stream.readLine();
           QStringList item = line.split(";");
           if (item[sr->getArticleCol()-1] == "") continue;
               statement.bindValue(":article", item[sr->getArticleCol()-1]);
               statement.bindValue(":qty", item[sr->getItemCountCol()-1]);
               statement.exec();
            }
            stream.flush();
        }
    statement.exec("COMMIT");

    csvFile.close();
    csvFile.remove();
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
    fsw->addPath(xlsFilePath); //добавляем в watcher новый файл
    qDebug() << "новый файл " + xlsFilePath + " добавлен в уотчер";
}

void StoreUpdater::updateCsvFilePath(QString xlsFilePath)
{
    csvFilePath = xlsFilePath.replace(QRegularExpression(".xlsx?"), ".csv").replace("/", "\\");
}

