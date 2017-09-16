#ifndef DATAIMPORT_H
#define DATAIMPORT_H
#include "store-remainings/storeremainings.h"
#include "import/datawriter.h"
#include <qsqldatabase.h>

class DataImport
{
public:
    DataImport(StoreRemainings *sr);
    bool connect();
    bool convertXlsToCsv();
    bool createTxt();
    QString import(QString xlsFilePath);
protected:
    bool loadDataInFile(QSqlQuery &query);
    bool parceData();
private:
    StoreRemainings *sr;
    DataWriter prodWriter;
    QSqlDatabase conn;
    QFile *csvFile;
    QSqlDatabase getConnection();
};

#endif // DATAIMPORT_H
