#include "vitebskdataimport.h"
#include "import/csvreader.h"
#include "import/dataparcer.h"
#include <qregularexpression.h>
#include <qsqlquery.h>
#define DESCRIPTION_COL 1
#define SMID 9999


VitebskDataImport::VitebskDataImport(StoreRemainings *sr) :
    DataImport(sr)
{
}

bool VitebskDataImport::parceData()
{
    QList<int> productInfo = QList<int>() << sr->getArticleCol() - 1
                                          << DESCRIPTION_COL
                                          << sr->getItemCountCol() -1;
    int maxElem = *qMax(productInfo.begin(), productInfo.end());
    CsvReader cr = CsvReader(csvFile, sr->getStartRow() - 1, maxElem);
    DataParcer prodParcer(productInfo);
    if (cr.openCsv()) {
        while (!cr.atEnd()) {
            QString line = prodParcer.parceLine(cr.readLine());
            if (!line.isEmpty()) {
                QStringList items = line.split('\t');
                QString pid;
                if (items.count() > 2) {
                    pid = findPid(items.at(0), items.at(1));
                    if (pid.isEmpty()) {
                        unknownProducts.insert(items.at(0), items.at(1));
                        continue;
                    }
                }
                prodWriter.append(pid + '\t' + QString::number(SMID) + '\t' + items.at(2));
            }
        }
        return true;
    }
    return false;
}

QString VitebskDataImport::findPid(const QString article, const QString desc) const
{
    QSqlQuery query;
    QString pid;
    query.prepare("SELECT `pid`, `description` FROM `products` WHERE `art` = :art");
    query.bindValue(":art", article);    
    query.exec();
    if (query.size() <= 0) return pid;
    if (query.size() > 1) {
        /* если по артикулу получено несколько pid - собираем их и сравниваем
         * с нужным desc, предварительно удалив все символы [^A-zА-я0-9] */
        QStringList pidList, descList;
        while (query.next()) {
            pidList << query.value(0).toString();
            descList << query.value(1).toString();
        }
        for (int i = 0; i < pidList.count(); ++i) {
            if (cleanSpecSym(descList.at(i)) == cleanSpecSym(desc)) {
                return pidList.at(i);
            }
        }
        return QString();
    }
    query.next();
    return query.value(0).toString();
}

QString VitebskDataImport::cleanSpecSym(QString str) const
{
    return str.replace(QRegExp("[^A-zА-я0-9]"), "");
}

bool VitebskDataImport::loadDataInFile(QSqlQuery &query)
{
    if (query.exec("LOAD DATA INFILE '" + prodWriter.getFilePath()
                   + "' IGNORE INTO TABLE `store`(`pid`, `smid`, `count`)")) {
        return true;
    }
    return false;
}
