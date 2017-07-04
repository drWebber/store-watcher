#ifndef SQLCONNECTOR_H
#define SQLCONNECTOR_H
#include <QtSql/qsqldatabase.h>

class SqlConnector
{
public:
    SqlConnector();
    void connect();
    bool isOpen();
private:
    QSqlDatabase db;
};

#endif // SQLCONNECTOR_H
