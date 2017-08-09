#include "filters/sortfilteproxyrmodel.h"
#include <qdebug.h>

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

void SortFilterProxyModel::addFilterFixedString(int column, const QString &pattern)
{
    if(!filterMap.contains(column)){

        filterMap.insert(column, pattern);
        for(QMap<int, QString>::const_iterator it = filterMap.begin();
            it != filterMap.end();
            ++it){
        }
        invalidate();
    }
}

void SortFilterProxyModel::removeFilter()
{
    filterMap.clear();
    invalidate();
}

bool SortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (filterMap.isEmpty()) {
        return true;
    }

    bool ret = false;
    QMap<int, QString>::const_iterator it = filterMap.begin();
    for (; it  != filterMap.end(); ++it) {
        QModelIndex indx = sourceModel()->index(source_row, it.key(), source_parent);
        ret = (indx.data().toString() == it.value());
        if (!ret) {
            return ret;
        }
    }
    return ret;
}
