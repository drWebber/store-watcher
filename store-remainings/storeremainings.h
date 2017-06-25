#ifndef STOREREMAININGS_H
#define STOREREMAININGS_H
#include <qfile.h>

class StoreRemainings
{
public:
    StoreRemainings(int smid, int mid, QString dirPath, QString regExp, QString currentFilePath,
                    int startRow, int articleCol, int itemCountCol);
    void updateCurrentFile();
    QString getCurrentFilePath();
    int getSmid();
    int getStartRow();
    int getArticleCol();
    int getItemCountCol();
private:
    QString currentFilePath;
    int smid;
    int mid;
    QString dirPath;
    QString regExp;
    int startRow;
    int articleCol;
    int itemCountCol;
};

#endif // STOREREMAININGS_H
