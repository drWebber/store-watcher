#ifndef CSVREADER_H
#define CSVREADER_H

#include <qfile.h>
#include <qtextstream.h>

class CsvReader
{
public:
    CsvReader(QFile *csvFile, int startRow, int maxElem);
    ~CsvReader();
    QString readLine();
    bool atEnd();
    bool openCsv();
private:
    QFile *csvFile;
    QTextStream *stream;
    int startRow;
    int maxElem;
    void replaceLine(QString &line, QString &replacePatt);
    int commaCounter(QString line);
};

#endif // CSVREADER_H
