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
    CsvReader cr = CsvReader(csvFile, sr->getStartRow() - 1);
    QList<int> productInfo = QList<int>() << sr->getArticleCol() - 1
                                          << DESCRIPTION_COL
                                          << sr->getItemCountCol() -1;
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
    query.prepare("SELECT `pid` FROM `products` WHERE `art` = :art AND `description` = :desc");
    query.bindValue(":art", article);
    query.bindValue(":desc", desc);
    query.exec();
    return query.next() ? query.value(0).toString() : QString();
}

bool VitebskDataImport::loadDataInFile(QSqlQuery &query)
{
    if (query.exec("LOAD DATA INFILE '" + prodWriter.getFilePath()
                   + "' IGNORE INTO TABLE `store`(`pid`, `smid`, `count`)")) {
        return true;
    }
    return false;
}
