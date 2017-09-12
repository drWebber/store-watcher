#ifndef COLOREDSQLQUERYMODEL_H
#define COLOREDSQLQUERYMODEL_H

#include <qsqlquerymodel.h>

class ColoredSqlQueryModel : public QSqlQueryModel
{
public:
    ColoredSqlQueryModel(QObject *parent);
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
private:
    QVector<QColor*> *columnColor; //почуму указатель *columnColor
};

#endif // COLOREDSQLQUERYMODEL_H
