#include "dataimport.h"
#include "excel/xlsreader.h"
#include "import/dataparcer.h"
#include "import/csvreader.h"
#include <qsqlerror.h>
#include <qdebug.h>
#include <qsqlquery.h>
#include <qdatetime.h>
#include <qregularexpression.h>
#include <qcryptographichash.h>

DataImport::DataImport(StoreRemainings *sr)
{
    this->sr = sr;
    connName = QString(QCryptographicHash::hash(sr->getCurrentFilePath().toLocal8Bit(),
                                        QCryptographicHash::Md5).toHex());
}

DataImport::~DataImport()
{
    conn.close();
    QSqlDatabase::removeDatabase(connName);
}

bool DataImport::connect()
{
    conn = getConnection();
    return conn.isValid() ? true : false;
}

QSqlDatabase DataImport::getConnection()
{
    QSqlDatabase conn = QSqlDatabase::addDatabase("QMYSQL3", connName);
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
        qDebug() << "DataImport::createTxt() ошибка открытия файла: " + csvFile->fileName() + ".txt";
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
    return msg;
}

QHash<QString, QString> DataImport::getUnknownProducts() const
{
    return unknownProducts;
}

bool DataImport::loadDataInFile(QSqlQuery &query)
{
    if (query.exec("LOAD DATA INFILE '" + prodWriter.getFilePath()
                                    + "' IGNORE INTO TABLE `store`")) {
        return true;
    }
    return false;
}

bool DataImport::parceData()
{
    QList<int> productInfo = QList<int>() << sr->getArticleCol() - 1
                                          << sr->getItemCountCol() -1;
    int maxElem = *std::max_element(productInfo.begin(), productInfo.end());
    CsvReader cr = CsvReader(csvFile, sr->getStartRow() - 1, maxElem);

    QString smid = QString::number(sr->getSmid());
    DataParcer prodParcer(productInfo);
    if (cr.openCsv()) {
        const int COLUMN_COUNT = 3; //кол-во столбцов для встаки `pid`, `smid`, `count`
        //собираем вектор значений на запись в txt-файл для импорта
        QVector<QStringList> import;
        QString predicate;  // Хранит значения предиката IN() для выборки pid из бд
        while (!cr.atEnd()) {
            QString line = prodParcer.parceLine(cr.readLine());
            if (!line.isEmpty()) {
                QStringList row = line.split('\t');
                row.insert(1, smid);
                if (row.count() == COLUMN_COUNT) {
                    predicate.append("\"" + row.at(0) + "\",");
                    import.append(row);
                }
            }            
        }
        predicate.chop(1);

        // Собираем QMap<'article', 'pid> для дальнейшего поиска pid
        QMap<QString, QString> pidMap;
        // Выбираем `article`, `pid` из таблицы `products`
        QSqlQuery q;
        q.prepare("SELECT `art`, `pid` FROM `products` WHERE `mid` = :mid AND `art` "
                  "IN(" + predicate + ")");
        q.bindValue(":mid", sr->getMid());
        q.exec();
        while (q.next()) {
            pidMap.insert(q.value("art").toString(), q.value("pid").toString());
        }

        //пишем в txt
        QVectorIterator<QStringList> it(import);
        while (it.hasNext()) {
            QStringList row = it.next();
            if (row.count() == COLUMN_COUNT) {
                QString pid = pidMap.value(row.at(0));
                if (!pid.isEmpty()) {
                    QString line = pid + '\t' + row.at(1) + '\t' + row.at(2);
                    prodWriter.append(line);
                }
            }
        }
        return true;
    }
    return false;
}
