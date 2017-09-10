#include "storeupdater.h"
#include <qregularexpression.h>
#include <qfileinfo.h>
#include <qdatetime.h>
#include <qsystemtrayicon.h>
#include "excel/xlsreader.h"
#include "import/csvreader.h"
#include "import/dataparcer.h"
#include "import/datawriter.h"

#include <qdebug.h>

StoreUpdater::StoreUpdater(QFileSystemWatcher &fsw, StoreRemainings &sr)
{
    xlsFilePath = sr.getCurrentFilePath();
    updateCsvFilePath(xlsFilePath);
    this->fsw = &fsw;
    this->sr = &sr;
}

void StoreUpdater::update()
{
    qDebug() << getCurrentTime() << "обновление остатков" << sr->getCurrentFilePath();

    QSqlDatabase conn = getConnection();
    if (!conn.isValid()) {
        qDebug() << "Обновление" + sr->getCurrentFilePath() + "уже запущено";
        return;
    }

    XlsReader xr(new QFile(sr->getCurrentFilePath()));
    xr.openActiveWorkBook();
    QFile *csvFile = xr.saveAsCsv();
    xr.close();

    CsvReader cr(csvFile, sr->getStartRow() - 1);
    QList<int> productInfo = QList<int>() << sr->getArticleCol() - 1
                                          << sr->getItemCountCol() -1;
    DataParcer prodParcer(productInfo);
    DataWriter prodWriter;
    if (!prodWriter.open(csvFile->fileName() + ".txt")) {
        emit importError("Ошибка открытия файла" + csvFile->fileName() + ".txt");
        return;
    }

    QString smid = QString::number(sr->getSmid());

    if (cr.openCsv()) {
        while (!cr.atEnd()) {
            QString line = prodParcer.parceLine(cr.readLine());
            if (!line.isEmpty()) {
                line.replace(QRegularExpression("^(.+?)\t"), "\\1\t" + smid + '\t');
                prodWriter.append(line);
            }
        }
    } else {
        emit importError("Не удалось прочитать csv...");
        return;
    }
    prodWriter.flush();

    QSqlQuery query(conn);
    query.exec("SET autocommit=0");
    query.exec("START TRANSACTION");

    query.prepare("DELETE FROM store WHERE smid = :smid");
    query.bindValue(":smid", sr->getSmid());
    query.exec();

    if (!query.exec("LOAD DATA INFILE '" + prodWriter.getFilePath()
                                    + "' INTO TABLE `store`(@pid, `smid`, `count`) "
                                      "SET pid = (SELECT `pid` FROM `products` WHERE `art` = @pid AND `mid` = " + QString::number(sr->getMid()) + ")")) {
        emit importError(query.lastError().text());
    }

    //обновляем инфо о дате обновленного файла остатков (таблица StoreDate)
    QFileInfo fileInfo(xlsFilePath);
    query.prepare("UPDATE `store_date` SET date = :creationDate WHERE smid = :smid");
    query.bindValue(":creationDate", fileInfo.created().toString("yyyy-MM-dd"));
    query.bindValue(":smid", sr->getSmid());
    if (!query.exec()) {
        emit importError(query.lastError().text());
    }

    query.exec("COMMIT");
    conn.close();

    qDebug() << getCurrentTime() << "обновление остатков" << sr->getCurrentFilePath() << "завершено";
}

void StoreUpdater::run()
{
    int timeFreezeKoef = 1;
    do {
        QThread::sleep(15*timeFreezeKoef);
        sr->updateCurrentFile();
        timeFreezeKoef++;
    } while (!QFile::exists(sr->getCurrentFilePath()) && timeFreezeKoef < 20);
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
    emit updateFinished(sr);
}

void StoreUpdater::updateCsvFilePath(QString xlsFilePath)
{
    csvFilePath = xlsFilePath.replace(QRegularExpression(".xlsx?"), ".csv").replace("/", "\\");
}

QSqlDatabase StoreUpdater::getConnection()
{
    QSqlDatabase conn = QSqlDatabase::addDatabase("QMYSQL3", sr->getCurrentFilePath());
    conn.setHostName("localhost");
    conn.setDatabaseName("nordelectro");
    conn.setUserName("root");

    if (!conn.open()) {
        qDebug() << "Failed to connect!";
        return QSqlDatabase();
    }
    return conn;
}

QString StoreUpdater::getCurrentTime()
{
    return QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
}

