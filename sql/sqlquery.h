#ifndef SQLQUERY_H
#define SQLQUERY_H
#include <QtSql/qsqlquery.h>
#include <qvariant.h>

class SqlQuery
{
public:
    SqlQuery();
    QVariant getSingleVal(QSqlQuery query);
    QVector<QString> getSingleVals(QString query);
    QStringList getSingleValsList(QString query);
private:
    QSqlQuery *sqlquery;
};

#endif // SQLQUERY_H
