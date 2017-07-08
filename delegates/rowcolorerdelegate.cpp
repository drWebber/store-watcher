#include "rowcolorerdelegate.h"
#include <qdebug.h>
#include <qpainter.h>
#include <qpen.h>

RowColorerDelegate::RowColorerDelegate()
{

}

void RowColorerDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (grayWords.contains(index.data().toString())) {
        painter->setPen(QColor(Qt::gray));
        painter->drawText(option.rect.adjusted(3, 7, 20, 20), index.data().toString());
    } else {
        QItemDelegate::paint(painter, option, index);
    }
}

void RowColorerDelegate::addGrayWord(QString word)
{
    grayWords.append(word);
}

void RowColorerDelegate::removeGrayWord(QString word)
{
    grayWords.removeAll(word);
}
