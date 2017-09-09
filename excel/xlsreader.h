#ifndef XLSREADER_H
#define XLSREADER_H

#include <qfile.h>
#include <qaxobject.h>
#include <qaxwidget.h>
#include <qfileinfo.h>

class XlsReader
{
public:
    XlsReader(QFile *xlsFile);
    QFile *saveAsCsv();
    void openActiveWorkBook();
    void close();
private:
    QFile *xlsFile;
    QFileInfo xlsFileInfo;
    QAxObject *excel;
    QAxObject *workbooks;
    QAxObject *workbook;
};

#endif // XLSREADER_H
