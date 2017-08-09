#ifndef SORTFILTEPROXYRMODEL_H
#define SORTFILTEPROXYRMODEL_H
#include <qsortfilterproxymodel.h>

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortFilterProxyModel(QObject *parent = 0);
    void addFilterFixedString(int column, const QString &pattern);
    void removeFilter();

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
private:
    QMap<int, QString> filterMap;
};

#endif // SORTFILTEPROXYRMODEL_H
