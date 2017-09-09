#include "datawriter.h"
#include <qfileinfo.h>

#include <qmessagebox.h>
#include <qdebug.h>

DataWriter::DataWriter()
{

}

DataWriter::~DataWriter()
{
    //file.remove();
}

void DataWriter::append(QString line)
{
    *stream << line << '\n';
}

bool DataWriter::open(const QString &tmpFileName)
{
    file.setFileName(tmpFileName);
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    stream = new QTextStream(&file);
    stream->setCodec("UTF-8");

    return true;
}

void DataWriter::flush()
{
    stream->flush();
}

QString DataWriter::getFilePath()
{
    return QFileInfo(file).absoluteFilePath();
}
