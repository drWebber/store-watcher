#include "coloredsqlquerymodel.h"

#include <qcolor.h>
#include <qdebug.h>
ColoredSqlQueryModel::ColoredSqlQueryModel(QObject *parent) :
    QSqlQueryModel(parent)
{

}

QVariant ColoredSqlQueryModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextColorRole) {
        return *columnColor.at(index.row());
    }
    return QSqlQueryModel::data(index, role);
}

bool ColoredSqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::TextColorRole) {
        *columnColor.at(index.row()) = value.value<QColor>();
        return true;
    }
    return QSqlQueryModel::setData(index, value, role);
}

void ColoredSqlQueryModel::update()
{
    setQuery("SELECT sm.smid, mn.name, sm.storePlace, sd.date "
             "FROM `store_manufacturer` AS sm LEFT JOIN `store_date` AS sd ON sm.smid = sd.smid, `manufacturers` AS mn "
             "WHERE sm.mid = mn.mid");
    while (columnColor.count() < rowCount()) {
            columnColor.push_back(new QColor(Qt::black));
    }

}
