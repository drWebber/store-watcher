#include "dataparcer.h"
#include <qregularexpression.h>
#include <qdebug.h>

DataParcer::DataParcer(const QList<int> &columns)
{
    this->columns = columns;
}

QString DataParcer::parceLine(QString line)
{
    QString replacePatt = "#scol#";
    if (line.contains(QRegExp("\"(.+);(.+)\"", Qt::CaseSensitive))) {
        replaceLine(line, replacePatt);
    }

    QStringList items = line.split(";");
    QString values;

    int max = *std::max_element(columns.begin(), columns.end());

    if (items.count() > max) {
        int count = columns.count();
        for (int i = 0; i < count; ++i) {
            QString item = items.at(columns.at(i));
            if (item.isEmpty()) return QString();
            if (item.contains(replacePatt)) {
                item.replace(replacePatt, ";");
            }
            values.append(item);
            if (i != count - 1) {
                values.append('\t');
            }
        }
    }
    return values;
}

void DataParcer::replaceLine(QString &line, QString &pattern)
{
    while (line.contains(pattern)) {
        pattern.append('#');
    }
    line.replace(QRegularExpression("\"(.+);(.+)\""),
                 "\\1" + pattern + "\\2");
}
