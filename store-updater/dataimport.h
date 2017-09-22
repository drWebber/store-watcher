#ifndef DATAIMPORT_H
#define DATAIMPORT_H
#include "store-remainings/storeremainings.h"
#include "import/datawriter.h"
#include <qsqldatabase.h>

class DataImport
{
public:
    DataImport(StoreRemainings *sr);
    ~DataImport();
    bool connect();
    bool convertXlsToCsv();
    bool createTxt();
    QString import(QString xlsFilePath);
    QHash<QString, QString> getUnknownProducts() const;
protected:
    QFile *csvFile;
    StoreRemainings *sr;
    DataWriter prodWriter;
    QHash<QString, QString> unknownProducts;
    virtual bool loadDataInFile(QSqlQuery &query);
    virtual bool parceData();
private:
    QSqlDatabase conn;
    QString connName;
    QSqlDatabase getConnection();
};

#endif // DATAIMPORT_H
