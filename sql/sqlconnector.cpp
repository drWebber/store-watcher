#include "sqlconnector.h"
#include <qmessagebox.h>
#include <qdebug.h>
#include <qsqlerror.h>

SqlConnector::SqlConnector()
{

}

void SqlConnector::connect()
{
    db = QSqlDatabase::addDatabase("QMYSQL3");
    db.setHostName("localhost");
    db.setDatabaseName("nordelectro");
    db.setUserName("nordelectro");
    db.setPassword("by3CVUTCN04oYS1H");
    if (!db.open()){
        QMessageBox::critical(NULL, "Ошибка", db.lastError().text());
        qDebug() << db.lastError().text();
    }
}

bool SqlConnector::isOpen()
{
    return db.isOpen() ? true : false;
}

