#include "dataimport.h"
#include "excel/xlsreader.h"
#include "import/dataparcer.h"
#include "import/csvreader.h"
#include <qsqlerror.h>
#include <qdebug.h>
#include <qsqlquery.h>
#include <qdatetime.h>
#include <qregularexpression.h>

DataImport::DataImport(StoreRemainings *sr)
{
    this->sr = sr;
}

bool DataImport::connect()
{
    conn = getConnection();
    return conn.isValid() ? true : false;
}

QSqlDatabase DataImport::getConnection()
{
    QSqlDatabase conn = QSqlDatabase::addDatabase("QMYSQL3", sr->getCurrentFilePath());
    conn.setHostName("localhost");
    conn.setDatabaseName("nordelectro");
    conn.setUserName("root");

    if (!conn.open()) {
        qDebug() << "QSqlDatabase DataImport::getConnection - Failed to connect!";
        return QSqlDatabase();
    }
    return conn;
}

bool DataImport::convertXlsToCsv()
{
    XlsReader xr(new QFile(sr->getCurrentFilePath()));
    xr.openActiveWorkBook();
    csvFile = xr.saveAsCsv();
    xr.close();
    return csvFile->exists() ? true : false;
}

bool DataImport::createTxt()
{
    bool result = true;
    if (!prodWriter.open(csvFile->fileName() + ".txt")) {
        qDebug() << "DataImport::createTxt() ошибка открытия файла:" + csvFile->fileName() + ".txt";
        result = false;
    }

    if (!parceData()) {
        qDebug() << "DataImport::createTxt() ошибка парсинга файла: " + csvFile->fileName() + ".txt";
        result = false;
    }

    prodWriter.flush();
    return result;
}

QString DataImport::import(QString xlsFilePath)
{
    QSqlQuery query(conn);
    query.exec("SET autocommit=0");
    query.exec("START TRANSACTION");

    query.prepare("DELETE FROM store WHERE smid = :smid");
    query.bindValue(":smid", sr->getSmid());
    query.exec();

    if (!loadDataInFile(query)) {
        query.exec("ROLLBACK");
        conn.close();
        return query.lastError().text();
    }

    //обновляем инфо о дате обновленного файла остатков (таблица StoreDate)
    QFileInfo fileInfo(xlsFilePath);
    query.prepare("UPDATE `store_date` SET date = :creationDate WHERE smid = :smid");
    query.bindValue(":creationDate", fileInfo.created().toString("yyyy-MM-dd"));
    query.bindValue(":smid", sr->getSmid());

    QString msg;

    if (!query.exec()) {
        msg.append(query.lastError().text());
    }

    query.exec("COMMIT");
    conn.close();
    return msg;
}

QHash<QString, QString> DataImport::getUnknownProducts() const
{
    return unknownProducts;
}

bool DataImport::loadDataInFile(QSqlQuery &query)
{
    if (query.exec("LOAD DATA INFILE '" + prodWriter.getFilePath()
                                    + "' IGNORE INTO TABLE `store`(@pid, `smid`, `count`) "
                                      "SET pid = (SELECT `pid` FROM `products` WHERE `art` = @pid AND `mid` = " + QString::number(sr->getMid()) + ")")) {
        return true;
    }
    return false;
}

bool DataImport::parceData()
{
    CsvReader cr = CsvReader(csvFile, sr->getStartRow() - 1);
    QList<int> productInfo = QList<int>() << sr->getArticleCol() - 1
                                          << sr->getItemCountCol() -1;

    QString smid = QString::number(sr->getSmid());
    DataParcer prodParcer(productInfo);
    if (cr.openCsv()) {
        while (!cr.atEnd()) {
            QString line = prodParcer.parceLine(cr.readLine());
            if (!line.isEmpty()) {
                line.replace(QRegularExpression("^(.+?)\t"), "\\1\t" + smid + '\t');
                prodWriter.append(line);
            }
        }
        return true;
    }
    return false;
}
