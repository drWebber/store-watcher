#include "xls/xlsreader.h"
#include <qaxobject.h>
#include <qaxwidget.h>
#include <qmessagebox.h>
#define xlCSV 6

XlsReader::XlsReader()
{

}

void XlsReader::xlsToCsv(QString xlsFilePath, QString csvFilePath)
{
    if(!xlsFilePath.isEmpty()){
        QAxWidget excel("Excel.Application");

        QAxObject *workbooks = excel.querySubObject("WorkBooks");
        workbooks->dynamicCall("Open (const QString&)", xlsFilePath);

        QAxObject *workbook = excel.querySubObject("ActiveWorkBook");

        QAxObject *sheets = workbook->querySubObject("Worksheets");
        QAxObject *sheet = sheets->querySubObject("Item(int)", 1); //1 - "Лист 1"

        QAxObject *range = sheet->querySubObject("UsedRange");

        QAxObject *rows = range->querySubObject("Rows");
        int rowCount = rows->dynamicCall("Count()").toInt();

        workbook->dynamicCall("SaveAs (const QString&, const int&)", csvFilePath, xlCSV);

        excel.setProperty("DisplayAlerts", false);
        workbook->dynamicCall("Close (Boolean)", true);

        delete workbook;
        delete workbooks;

        excel.dynamicCall("Quit (void)");
    } else {
        QMessageBox::critical(NULL, "Ошибка открытия файла", "Путь к xls файлу пуст");
    }
}
