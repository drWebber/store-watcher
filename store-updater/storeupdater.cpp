#include "storeupdater.h"
#include "xls/xlsreader.h"
#include <qregularexpression.h>
#include <qdebug.h>
#include <qsqlquery.h>
#include <qfileinfo.h>
#include <qdatetime.h>
#include <qsystemtrayicon.h>

StoreUpdater::StoreUpdater(StoreRemainings &sr)
{
    this->xlsFilePath = sr.getCurrentFilePath();
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

    QSqlQuery statement;
    statement.exec("START TRANSACTION");

    //удаляем устаревшие остатки
    statement.prepare("DELETE FROM store WHERE smid = :smid");
    statement.bindValue(":smid", sr->getSmid());
    statement.exec();

    //заливаем новые остатки
    statement.prepare("INSERT INTO store(pid, smid, count) VALUES ((SELECT pid FROM products WHERE art = :article), :smid, :qty)");
    //statement.prepare("INSERT INTO store(pid, smid, count) VALUES (:article, :smid, :qty)");

    //читаем csv
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
               statement.bindValue(":smid", sr->getSmid());
               statement.bindValue(":qty", item[sr->getItemCountCol()-1]);
               statement.exec();
            }
            stream.flush();
        }
    statement.exec("COMMIT");
    csvFile.close();
    csvFile.remove();

    //обновляем инфо о дате обновленного файла остатков (таблица StoreDate)
    QFileInfo fileInfo(xlsFilePath);
    statement.prepare("UPDATE StoreDate SET date = :creationDate WHERE smid = :smid");
    statement.bindValue(":creationDate", fileInfo.created().toString("dd.MM.yyyy"));
    statement.bindValue(":smid", sr->getSmid());
    statement.exec();
}

void StoreUpdater::run()
{
    qDebug() << "yeeeehhh, we are in thread";
    int timeFreezeKoef = 1;
    do {
        QThread::sleep(10*timeFreezeKoef);
        sr->updateCurrentFile();
        timeFreezeKoef++;
        qDebug() << "timeFreezeKoef = " + QString::number(timeFreezeKoef);
    } while (!QFile::exists(sr->getCurrentFilePath()) && timeFreezeKoef < 10);
    if(QFile::exists(sr->getCurrentFilePath())){
        xlsFilePath = sr->getCurrentFilePath();
        updateCsvFilePath(xlsFilePath);
        update();
        fsw->addPath(xlsFilePath); //добавляем в watcher новый файл
        qDebug() << "новый файл " + xlsFilePath + " добавлен в watcher";
    } else {
        QSqlQuery query = QSqlQuery();
        query.prepare("SELECT mn.name, sm.storePlace "
                      "FROM store_manufacturer AS sm INNER JOIN manufacturers AS mn ON sm.mid = mn.mid "
                      "WHERE sm.smid = :smid");
        query.bindValue(":smid", sr->getSmid());
        query.exec();
        query.next();

        QSystemTrayIcon trayIcon;
        trayIcon.setVisible(true);
        trayIcon.showMessage("Файл не найден", "Файл остатков " + query.value(0).toString() +
                                 " " + query.value(1).toString() + " не найден");
    }
}

void StoreUpdater::updateCsvFilePath(QString xlsFilePath)
{
    csvFilePath = xlsFilePath.replace(QRegularExpression(".xlsx?"), ".csv").replace("/", "\\");
}

