#include "rowcolorerdelegate.h"
#include <qdebug.h>
#include <qpainter.h>
#include <qpen.h>

RowColorerDelegate::RowColorerDelegate(QSortFilterProxyModel &proxy)
{
    this->proxy = &proxy;
}

void RowColorerDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    int rowData = proxy->index(index.row(), 0).data().toInt();
    if (smIdList.contains(rowData) || colorRow == index.row()) {
        int colorIndex = smIdList.indexOf(rowData);
        painter->setPen(QColor(colorList.at(colorIndex)));
        painter->drawText(option.rect.adjusted(3, 7, 20, 20), index.data().toString());
    } else {
        colorRow = -1;
        QItemDelegate::paint(painter, option, index);
    }
}

void RowColorerDelegate::setSmIdColor(int smid, QString colorName)
{
    smIdList.append(smid);
    colorList.append(colorName);
}

void RowColorerDelegate::unsetSmidColor(int smid)
{
    int indx = smIdList.indexOf(smid);
    smIdList.removeAll(smid);
    colorList.removeAt(indx);
}
