#ifndef SEARCHRESULTVIEWITEMDELEGATE_H
#define SEARCHRESULTVIEWITEMDELEGATE_H

#include <QStyledItemDelegate>

class SearchResultViewItemDelegate : public QStyledItemDelegate
{
public:
    explicit SearchResultViewItemDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // SEARCHRESULTVIEWITEMDELEGATE_H