#ifndef GAMS_STUDIO_GDXVIEWER_GDXVIEWERSTATE_H
#define GAMS_STUDIO_GDXVIEWER_GDXVIEWERSTATE_H

#include "gdxsymbolviewstate.h"

#include <QMap>
#include <QObject>

namespace gams {
namespace studio {
namespace gdxviewer {

class GdxViewerState
{
public:
    GdxViewerState();
    ~GdxViewerState();

    GdxSymbolViewState* symbolViewState(QString name) const;
    GdxSymbolViewState* addSymbolViewState(QString name);
    void deleteSymbolViewState(QString name);

    QByteArray symbolTableHeaderState() const;
    void setSymbolTableHeaderState(const QByteArray &symbolTableHeaderState);

    QMap<QString, GdxSymbolViewState *> symbolViewStates() const;

    QString selectedSymbol() const;
    void setSelectedSymbol(const QString &selectedSymbol);

    bool selectedSymbolIsAlias() const;
    void setSelectedSymbolIsAlias(bool selectedSymbolIsAlias);

private:
    QByteArray mSymbolTableHeaderState;
    QMap<QString, GdxSymbolViewState*> mSymbolViewState;
    QString mSelectedSymbol = "";
    bool mSelectedSymbolIsAlias = false;
};

} // namespace gdxviewer
} // namespace studio
} // namespace gams

#endif // GAMS_STUDIO_GDXVIEWER_GDXVIEWERSTATE_H
