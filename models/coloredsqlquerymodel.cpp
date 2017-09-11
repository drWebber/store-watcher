#include "coloredsqlquerymodel.h"

#include <qcolor.h>
#include <qdebug.h>
ColoredSqlQueryModel::ColoredSqlQueryModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    columnColor = new QVector<QColor*>;
}

QVariant ColoredSqlQueryModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextColorRole) {
        while (columnColor->count() < columnCount()) {
                columnColor->push_back(new QColor(Qt::black));
        }
        return *columnColor->at(index.row());
    }
    return QSqlQueryModel::data(index, role);
}

bool ColoredSqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::TextColorRole) {
        *columnColor->at(index.row()) = value.value<QColor>();
        return true;
    }
    return QSqlQueryModel::setData(index, value, role);
}
