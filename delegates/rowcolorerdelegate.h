#ifndef ROWCOLORERDELEGATE_H
#define ROWCOLORERDELEGATE_H
#include <qitemdelegate.h>
#include <qsortfilterproxymodel.h>

class RowColorerDelegate : public QItemDelegate
{
public:
    RowColorerDelegate(QSortFilterProxyModel &proxy);
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setSmIdColor(int smid, QString colorName);
    void unsetSmidColor(int smid);
private:
    QList<int> smIdList;
    QList<QString> colorList;
    mutable int colorRow = -1;
    QSortFilterProxyModel *proxy;
};

#endif // ROWCOLORERDELEGATE_H
