#include "dataparcer.h"

#include <qdebug.h>

DataParcer::DataParcer(const QList<int> &columns)
{
    this->columns = columns;
    std::sort(this->columns.begin(), this->columns.end());
}

QString DataParcer::parceLine(const QString &line)
{
    QStringList items = line.split(";");
    QString values;
    if (items.count() > columns.last()) {
        int count = columns.count();
        for (int i = 0; i < count; ++i) {
            QString item = items.at(i);
            if (item.isEmpty()) return QString();
            values.append(item);
            if (i != count - 1) {
                values.append('\t');
            }
        }
    }
    return values;
}
