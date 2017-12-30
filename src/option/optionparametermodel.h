#ifndef OPTIONPARAMETERMODEL_H
#define OPTIONPARAMETERMODEL_H

#include "commandlinetokenizer.h"
#include "option.h"

namespace gams {
namespace studio {

class OptionParameterModel : public QAbstractTableModel
{
     Q_OBJECT
public:
    OptionParameterModel(const QString normalizedCommandLineStr, CommandLineTokenizer* tokenizer, QObject *parent = 0);

    QVariant headerData(int index, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setHeaderData(int index, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

signals:
    void editCompleted(const QString &commandLineStr);

private:
    QList<OptionItem> mOptionItem;
    QList<QString> mHeader;
    QMap<int, QVariant> mCheckState;

    CommandLineTokenizer* commandLineTokenizer;
    Option* gamsOption;
};

} // namespace studio
} // namespace gams

#endif // OPTIONPARAMETERMODEL_H
