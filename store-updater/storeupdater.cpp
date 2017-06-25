#include "storeupdater.h"
#include "xls/xlsreader.h"
#include <qregularexpression.h>
#include <qdebug.h>
StoreUpdater::StoreUpdater(QString xlsFilePath)
{
    this->xlsFilePath = xlsFilePath;
    csvFilePath = xlsFilePath.replace(QRegularExpression(".xlsx?"), ".csv").replace("/", "\\");
}

void StoreUpdater::update()
{
    qDebug() << "update event";
    XlsReader *xlsReader = new XlsReader();
    xlsReader->xlsToCsv(xlsFilePath, csvFilePath);
}

