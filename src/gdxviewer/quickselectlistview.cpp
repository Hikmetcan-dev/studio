/*
 * This file is part of the GAMS Studio project.
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>
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
#include "columnfilterframe.h"
#include "quickselectlistview.h"
#include <QMouseEvent>

namespace gams {
namespace studio {
namespace gdxviewer {

QuickSelectListView::QuickSelectListView(QWidget *parent) :
    QListView(parent)
{

}

void QuickSelectListView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton || (event->button() == Qt::LeftButton && event->modifiers() & Qt::ControlModifier)) {
        QModelIndex idx = this->indexAt(event->pos());
        if (idx.isValid()) {
            for(int row=0; row<model()->rowCount(); row++)
                model()->setData(model()->index(row,0), false, Qt::CheckStateRole);
            this->model()->setData(idx, true, Qt::CheckStateRole);
            emit quickSelect();
        }
        event->accept();
    }
    else
        QListView::mouseReleaseEvent(event);
}


} // namespace gdxviewer
} // namespace studio
} // namespace gams
