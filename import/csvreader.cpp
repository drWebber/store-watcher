#include "csvreader.h"

CsvReader::CsvReader(QFile *csvFile, int startRow)
{
    this->csvFile = csvFile;
    this->startRow = startRow;
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
        line = stream->readLine();
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
