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
#ifndef GAMS_STUDIO_GDXVIEWER_EXPORTDRIVER_H
#define GAMS_STUDIO_GDXVIEWER_EXPORTDRIVER_H

#include "exportmodel.h"

#include <QString>
#include <memory>
#include <process/connectprocess.h>


namespace gams {
namespace studio {
namespace gdxviewer {

class ExportDriver : public QObject
{
    Q_OBJECT

public:
    explicit ExportDriver(GdxViewer *gdxViewer, ExportModel *exportModel, QObject *parent = nullptr);
    ~ExportDriver();
    bool save(QString connectFile, QString output, bool applyFilters);
    void execute(QString connectFile, QString workingDirectory);
    void saveAndExecute(QString connectFile, QString output, QString workingDirectory, bool applyFilters);
    void cancelProcess(int waitMSec=0);

signals:
    void exportDone();

private:
    const QString PROJ_SUFFIX = "_proj_";
    const QString FILTER_SUFFIX = "_filter_";

    std::unique_ptr<ConnectProcess> mProc = nullptr;
    GdxViewer *mGdxViewer = nullptr;
    ExportModel *mExportModel = nullptr;
    QString generateInstructions(QString gdxFile, QString output, bool applyFilters);
    QString generateGdxReader(QString gdxFile);
    QString generatePDExcelWriter(QString excelFile, bool applyFilters);
    QString generateProjections(bool applyFilters);
    QString generateFilters();

    QString generateDomains(GdxSymbol *sym);
    QString generateDomainsNew(GdxSymbol *sym);

    bool hasActiveLabelFilter(GdxSymbol *sym);
    bool hasActiveValueFilter(GdxSymbol *sym);
    bool hasActiveFilter(GdxSymbol *sym);

};

} // namespace gdxviewer
} // namespace studio
} // namespace gams

#endif // GAMS_STUDIO_GDXVIEWER_EXPORTDRIVER_H
