#ifndef VITEBSKDATAIMPORT_H
#define VITEBSKDATAIMPORT_H

#include "dataimport.h"



class VitebskDataImport : public DataImport
{
public:
    VitebskDataImport(StoreRemainings *sr);
protected:
//    bool loadDataInFile(QSqlQuery &query);
    bool parceData();
private:
    QString findPid(const QString article, const QString desc) const;
    bool loadDataInFile(QSqlQuery &query) override;
};

#endif // VITEBSKDATAIMPORT_H
