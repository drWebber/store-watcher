#ifndef CSVREADER_H
#define CSVREADER_H

#include <qfile.h>
#include <qtextstream.h>

class CsvReader
{
public:
    CsvReader(QFile *csvFile, int startRow);
    ~CsvReader();
    QString readLine();
    bool atEnd();
    bool openCsv();
private:
    QFile *csvFile;
    QTextStream *stream;
    int startRow;
};

#endif // CSVREADER_H
