#include "storeremainings.h"

#include <qdir.h>
#include <qregularexpression.h>
#include <qsqlquery.h>

StoreRemainings::StoreRemainings()
{

}

void StoreRemainings::updateCurrentFile()
{
    currentFilePath = "";
    QDir dir;
    dir.setPath(dirPath);
    QStringList files = dir.entryList();
    foreach (QString file, files) {
        if (file.contains(QRegularExpression(regExp))) {
            currentFilePath = QFile(dir.absoluteFilePath(file)).fileName();
        }
    }
    QSqlQuery query;
    query.prepare("UPDATE store_manufacturer SET lastPath = :lastpath WHERE smid = :smid");
    query.bindValue(":lastpath", currentFilePath);
    query.bindValue(":smid", smid);
    query.exec();
}

QString StoreRemainings::getCurrentFilePath() const
{
    return currentFilePath;
}

void StoreRemainings::setCurrentFilePath(const QString &value)
{
    currentFilePath = value;
}

QString StoreRemainings::getStorePlace() const
{
    return storePlace;
}

void StoreRemainings::setStorePlace(const QString &value)
{
    storePlace = value;
}

QList<StoreRemainings *> *StoreRemainings::loadRemainings()
{
    QList<StoreRemainings *> *rems = new QList<StoreRemainings *>();
    QSqlQuery query;
    query.exec("SELECT * FROM store_manufacturer");
    while (query.next()) {
        StoreRemainings *sr = new StoreRemainings();
        sr->setSmid(query.value("smid").toInt());
        int mid = query.value("mid").toInt();
        sr->setMid(mid);
        sr->setManufacturer(getManufacturerName(mid));
        sr->setDirPath(query.value("path").toString());
        sr->setRegExp(query.value("regexp").toString());
        sr->setCurrentFilePath(query.value("lastPath").toString());
        sr->setStartRow(query.value("startRow").toInt());
        sr->setArticleCol(query.value("articleCol").toInt());
        sr->setItemCountCol(query.value("itemCountCol").toInt());
        sr->setStorePlace(query.value("storePlace").toString());
        rems->push_back(sr);
    }
    return rems;
}

QString StoreRemainings::getDirPath() const
{
    return dirPath;
}

void StoreRemainings::setDirPath(const QString &value)
{
    dirPath = value;
}

QString StoreRemainings::getRegExp() const
{
    return regExp;
}

void StoreRemainings::setRegExp(const QString &value)
{
    regExp = value;
}

int StoreRemainings::getSmid() const
{
    return smid;
}

void StoreRemainings::setSmid(int value)
{
    smid = value;
}

int StoreRemainings::getMid() const
{
    return mid;
}

void StoreRemainings::setMid(int value)
{
    mid = value;
}

int StoreRemainings::getStartRow() const
{
    return startRow;
}

void StoreRemainings::setStartRow(int value)
{
    startRow = value;
}

int StoreRemainings::getArticleCol() const
{
    return articleCol;
}

void StoreRemainings::setArticleCol(int value)
{
    articleCol = value;
}

int StoreRemainings::getItemCountCol() const
{
    return itemCountCol;
}

void StoreRemainings::setItemCountCol(int value)
{
    itemCountCol = value;
}

QString StoreRemainings::getManufacturer() const
{
    return manufacturer;
}

void StoreRemainings::setManufacturer(const QString &value)
{
    manufacturer = value;
}

QString StoreRemainings::getManufacturerName(int mid)
{
    QSqlQuery q;
    q.prepare("SELECT `name` FROM `manufacturers` WHERE `mid` = :mid");
    q.bindValue(":mid", mid);
    q.exec();
    if (q.next()) {
        return q.value(0).toString();
    }
    return QString("getManufacturerName error");
}
