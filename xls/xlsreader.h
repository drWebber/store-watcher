#ifndef XlsReader_H
#define XlsReader_H
#include <qstring.h>

class XlsReader
{
public:
    XlsReader();
    void xlsToCsv(QString xlsFilePath, QString csvFilePath);
};

#endif // XlsReader_H
