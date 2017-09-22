#ifndef DATAPARCER_H
#define DATAPARCER_H

#include <qlist.h>



class DataParcer
{
public:
    DataParcer(const QList<int> &columns);
    QString parceLine(QString line);
private:
    int maxElem;
    QList<int> columns;
    void replaceLine(QString &line, QString &replacePatt);
    int count;
};

#endif // DATAPARCER_H
