#include "dataparcer.h"
#include <qregexp.h>
#include <qdebug.h>

DataParcer::DataParcer(const QList<int> &columns)
{
    this->columns = columns;    
    maxElem = *std::max_element(columns.begin(), columns.end());
    count = columns.count();
}

QString DataParcer::parceLine(QString line)
{
    QString replacePatt = "#scol#";
    if (line.contains(QRegExp("\"(.+);(.+)\"", Qt::CaseSensitive))) {
        replaceLine(line, replacePatt);
    }
    QStringList items = line.split(";");
    QString values;
    for (int i = 0; i < count; ++i) {
        QString item = items.at(columns.at(i));
        if (item.isEmpty()) return QString();
        if (item.contains(replacePatt)) {
            item.replace(replacePatt, ";");
        }

        //  заменяем пару двойных кавычек на просто двойные
        if (item.contains(QRegExp("\"\""))) {
            item.replace(QRegExp("\"\""), "\"");
        }
        /* если строка содержит QRegExp("^\"(.+?)\"$") - вставляем excel
         * при сохранении в csv когда в ячейке встречаются ';'
         * то удаляем эти кавычки */
        if (item.left(1) == '"' && item.right(1) == '"') {
            item.remove(0, 1);
            item.chop(1);
        }

        // удаляем пробелы в начале и в конце ячейки, если они есть
        if (item.left(1) == ' ' && item.right(1) == ' ') item = item.trimmed();
        // удаляем пробелы в конце ячейки, если они есть, в начале не удаляем
        // т.к. они должны остаться (так делает 1С)
        while (item.right(1) == ' ') {
            item.chop(1);
        }

        values.append(item);
        if (i != count - 1) {
            values.append('\t');
        }
    }
    return values.replace("\\", "\\\\");
}

void DataParcer::replaceLine(QString &line, QString &replacePatt)
{
    /* если паттерн вдруг встретится в строке, добавляем к нему
       шарпы, пока он не станет уникальным. */
    while (line.contains(replacePatt)) {
        replacePatt.append('#');
    }

    /* если в строке встречаем шаблон \"(.+;.+)\" - есть вероятность что
     * симолов ';' может быть несколько, поэтому отдельно "вынимаем" по
     * шаблону подстроку, меняем в ней ';' на replacePatt, далее заменяем
     * подстроку в исходной */
    QRegExp quotesRx("\"(.+;.+)\"");
    int startIndex = line.indexOf(quotesRx);
    if (startIndex != -1) {
        int lenght = quotesRx.cap(0).length();
        QString mid = line.mid(startIndex, lenght);
        mid.replace(";", replacePatt);
        line.replace(startIndex, lenght, mid);
    }
}
