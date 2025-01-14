/*
 * This file is part of the GAMS Studio project.
 *
 * Copyright (c) 2017-2023 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2023 GAMS Development Corp. <support@gams.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef REFERENCEVIEWER_H
#define REFERENCEVIEWER_H

#include <QWidget>
#include <QList>
#include <QMap>
#include <QTabWidget>
#include "common.h"
#include "reference.h"
#include "abstractview.h"

namespace Ui {
class ReferenceViewer;
}

namespace gams {
namespace studio {
namespace reference {

enum class ReferenceViewerType {
    undefined = 0,
    Symbol,
    FileUsed
};

class Reference;
class ReferenceTabStyle;
class ReferenceViewer : public AbstractView
{
    Q_OBJECT

public:
    explicit ReferenceViewer(QString referenceFile, QStringConverter::Encoding encoding, QWidget *parent = nullptr);
    ~ReferenceViewer() override;
    void selectSearchField() const;
    void updateStyle();

signals:
    void jumpTo(gams::studio::reference::ReferenceItem item);

public slots:
    void on_referenceFileChanged(QStringConverter::Encoding encoding);
    void on_tabBarClicked(int index);
    void updateView(bool status);

private:
    Ui::ReferenceViewer *ui;
    QStringConverter::Encoding mEncoding;
    QScopedPointer<Reference> mReference;
    QScopedPointer<ReferenceTabStyle> mRefTabStyle;
};

} // namespace reference
} // namespace studio
} // namespace gams

#endif // REFERENCEVIEWER_H
