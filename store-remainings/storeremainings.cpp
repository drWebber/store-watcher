#include "storeremainings.h"

#include <qdir.h>
#include <qregularexpression.h>
#include <qsqlquery.h>

StoreRemainings::StoreRemainings(int smid, int mid, QString dirPath, QString regExp,
                                 QString currentFilePath, int startRow, int articleCol,
                                 int itemCountCol)
{
    this->smid = smid;
    this->mid = mid;
    this->dirPath = dirPath;
    this->regExp = regExp;
    this->currentFilePath = currentFilePath;
    this->startRow = startRow;
    this->articleCol = articleCol;
    this->itemCountCol = itemCountCol;
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

QString StoreRemainings::getCurrentFilePath()
{
    return currentFilePath;
}

int StoreRemainings::getSmid()
{
    return smid;
}

int StoreRemainings::getStartRow()
{
    return startRow;
}

int StoreRemainings::getArticleCol()
{
    return articleCol;
}

int StoreRemainings::getItemCountCol()
{
    return itemCountCol;
}

int StoreRemainings::getMid() const
{
    return mid;
}
