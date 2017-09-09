#ifndef DATAWRITER_H
#define DATAWRITER_H

#include <qfile.h>
#include <qtextstream.h>



class DataWriter
{
public:
    DataWriter();
    ~DataWriter();
    bool open(const QString &tmpFileName);
    QString getFilePath();
    void append(QString line);
    void flush();
private:
    QFile file;
    QTextStream *stream;
};

#endif // DATAWRITER_H
