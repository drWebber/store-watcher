#ifndef STOREREMAININGS_H
#define STOREREMAININGS_H
#include <qfile.h>

class StoreRemainings
{
public:
    StoreRemainings();
    static QList<StoreRemainings *> *loadRemainings();
    void updateCurrentFile();
    QString getCurrentFilePath() const;
    void setCurrentFilePath(const QString &value);

    QString getStorePlace() const;
    void setStorePlace(const QString &value);

    QString getDirPath() const;
    void setDirPath(const QString &value);

    QString getRegExp() const;
    void setRegExp(const QString &value);

    int getSmid() const;
    void setSmid(int value);

    int getMid() const;
    void setMid(int value);

    int getStartRow() const;
    void setStartRow(int value);

    int getArticleCol() const;
    void setArticleCol(int value);

    int getItemCountCol() const;
    void setItemCountCol(int value);

    QString getManufacturer() const;
    void setManufacturer(const QString &value);
private:
    QString currentFilePath, storePlace, dirPath, regExp, manufacturer;
    int smid, mid, startRow, articleCol, itemCountCol;
    static QString getManufacturerName(int mid);
};

#endif // STOREREMAININGS_H
