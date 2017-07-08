#ifndef ROWCOLORERDELEGATE_H
#define ROWCOLORERDELEGATE_H
#include <qitemdelegate.h>

class RowColorerDelegate : public QItemDelegate
{
public:
    RowColorerDelegate();
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void addGrayWord(QString word);
    void removeGrayWord(QString word);
private:
    QStringList grayWords;
};

#endif // ROWCOLORERDELEGATE_H
