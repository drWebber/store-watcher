#include "csvreader.h"

#include <qdebug.h>

CsvReader::CsvReader(QFile *csvFile, int startRow, int maxElem)
{
    this->csvFile = csvFile;
    this->startRow = startRow;
    this->maxElem = maxElem;
}

CsvReader::~CsvReader()
{
    csvFile->close();
    csvFile->remove();
}

QString CsvReader::readLine()
{
    QString line;
    while (line.isEmpty()) {
        line.append(stream->readLine());
    }
    QString replacePatt = "#scol#";
    if (line.contains(QRegExp("\"(.+);(.+)\"", Qt::CaseSensitive))) {
        replaceLine(line, replacePatt);
    }

    int count = commaCounter(line);
    while (count < maxElem) {
        line.append(" " + stream->readLine());
        count = commaCounter(line);
    }

    if (line.contains(replacePatt)) {
        line.replace(replacePatt, ";");
    }

    return line;
}

bool CsvReader::atEnd()
{
    return stream->atEnd() ? true : false;
}

bool CsvReader::openCsv()
{
    if (csvFile->open(QFile::ReadOnly | QFile::Text)) {
        stream = new QTextStream(csvFile);
        for (int i = 0; i < startRow; ++i) stream->readLine();
        return true;
    } else {
        return false;
    }
}

void CsvReader::replaceLine(QString &line, QString &replacePatt)
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

int CsvReader::commaCounter(QString line)
{
    QRegExp rx(";");
    int count = 0;
    int pos = 0;
    while ((pos = rx.indexIn(line, pos)) != -1) {
        ++count;
        pos += rx.matchedLength();
    }
    return count;
}
