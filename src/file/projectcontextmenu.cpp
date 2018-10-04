/*
 * This file is part of the GAMS Studio project.
 *
 * Copyright (c) 2017-2018 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2018 GAMS Development Corp. <support@gams.com>
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
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include "projectcontextmenu.h"
#include "file.h"
#include "commonpaths.h"

namespace gams {
namespace studio {

enum ContextAction {
    actExplorer,
    actLogTab,
    actRename,
    actSep1,
    actSetMain,
    actSep2,
    actAddExisting,
    actAddNew,
    actSep3,
    actCloseGroup,
    actCloseFile,
};

ProjectContextMenu::ProjectContextMenu()
{
    mActions.insert(actExplorer, addAction("&Open location", this, &ProjectContextMenu::onOpenFileLoc));
    mActions.insert(actLogTab, addAction("&Open log tab", this, &ProjectContextMenu::onOpenLog));
    mActions.insert(actRename, addAction("Re&name",  this, &ProjectContextMenu::onRenameGroup));
    mActions.insert(actSep1, addSeparator());

    mActions.insert(actSetMain, addAction("&Set as main file", this, &ProjectContextMenu::onSetMainFile));
    mActions.insert(actSep2, addSeparator());

    mActions.insert(actAddExisting, addAction("Add &existing file", this, &ProjectContextMenu::onAddExisitingFile));
    mActions.insert(actAddNew, addAction("Add &new file", this, &ProjectContextMenu::onAddNewFile));
    mActions.insert(actSep3, addSeparator());

    mActions.insert(actCloseGroup, addAction("Close &group", this, &ProjectContextMenu::onCloseGroup));
    mActions.insert(actCloseFile, addAction("Close &file", this, &ProjectContextMenu::onCloseFile));

    mActions.insert(11, addSeparator());

    //    mActions.insert(2, addSeparator());
//    mActions.insert(2, addAction("Re&name",  this, &ProjectContextMenu::onRenameFile));

    QMenu* newSolverOptionMenu = addMenu( "Create Solver Option File" );

    int solverOptActionBaseIndex = 100;
    mActions.insert(solverOptActionBaseIndex, newSolverOptionMenu->menuAction());

    QDir sysdir(CommonPaths::systemDir());
    QStringList optFiles = sysdir.entryList(QStringList() << "opt*.def" , QDir::Files);
    foreach(QString filename, optFiles) {
        QString solvername = filename.mid(QString("opt").length());
        solvername.replace(QRegExp(".def"), "");
        if (QString::compare("gams", solvername ,Qt::CaseInsensitive)==0)
            continue;

        QAction* createSolverOption = newSolverOptionMenu->addAction(solvername);
        connect(createSolverOption, &QAction::triggered, [=] { createSolverOptionFile(solvername, filename); });


        mSolverOptionActions.insert(++solverOptActionBaseIndex, createSolverOption);
    }
    connect(this, &ProjectContextMenu::createSolverOptionFile, this, &ProjectContextMenu::onCreateSolverOptionFile );

}

bool ProjectContextMenu::setNodes(ProjectAbstractNode *current, QVector<ProjectAbstractNode *> selected)
{
    // synchronize current and selected
    mNodes.clear();
    if (!current && selected.count() == 1) current = selected.first();
    if (selected.count() < 2) {
        mNodes << current;
    } else {
        mNodes = selected;
    }
    mNodes = selected;
    if (mNodes.isEmpty()) return false;
    ProjectAbstractNode *one = mNodes.count()==1 ? mNodes.first() : nullptr;

    bool isGroup = mNodes.first()->toGroup();
    ProjectFileNode *fileNode = mNodes.first()->toFile();
    bool isGmsFile = fileNode && fileNode->file()->kind() == FileKind::Gms; // unused
    bool isRunnable = false;

    QString file;
    if (fileNode && fileNode->assignedRunGroup()) {
        file = fileNode->assignedRunGroup()->specialFile(FileKind::Gms);
        isRunnable = fileNode->location() == file;
    }

    mActions[actExplorer]->setEnabled(one);
    mActions[actLogTab]->setVisible(isGroup);
    mActions[actLogTab]->setEnabled(one);
    mActions[actRename]->setVisible(isGroup);
    mActions[actRename]->setEnabled(one);
    mActions[actSep1]->setVisible(isGroup);
    mActions[actSetMain]->setVisible(isGmsFile && !isRunnable);

    // all files
    mActions[actCloseFile]->setVisible(fileNode);
    return true;
}

void ProjectContextMenu::onCloseFile()
{
    for (ProjectAbstractNode *node: mNodes) {
        ProjectFileNode *file = node->toFile();
        if (file) emit closeFile(file);
    }
}

void ProjectContextMenu::onAddExisitingFile()
{
    QString sourcePath = "";
    emit getSourcePath(sourcePath);

    QStringList filePaths = QFileDialog::getOpenFileNames(mParent,
                                                    "Add existing file",
                                                    sourcePath,
                                                    tr("GAMS code (*.gms *.inc *.gdx *.lst *.opt *ref);;"
                                                       "Text files (*.txt);;"
                                                       "All files (*.*)"),
                                                    nullptr,
                                                    DONT_RESOLVE_SYMLINKS_ON_MACOS);
    if (filePaths.isEmpty()) return;

    QVector<ProjectGroupNode*> groups;
    for (ProjectAbstractNode *node: mNodes) {
        ProjectGroupNode *group = node->toGroup();
        if (!group) group = node->parentNode();
        if (!groups.contains(group))
            groups << group;
    }
    for (ProjectGroupNode *group: groups) {
        for (QString filePath: filePaths) {
            emit addExistingFile(group, filePath);
        }
    }
}

void ProjectContextMenu::onAddNewFile()
{
    QString sourcePath = "";
    emit getSourcePath(sourcePath);

    QString filePath = QFileDialog::getSaveFileName(mParent,
                                                    "Create new file...",
                                                    sourcePath,
                                                    tr("GAMS code (*.gms *.inc);;"
                                                       "Text files (*.txt);;"
                                                       "All files (*.*)"),
                                                    nullptr,
                                                    DONT_RESOLVE_SYMLINKS_ON_MACOS);

    if (filePath == "") return;

    QFileInfo fi(filePath);
    if (fi.suffix().isEmpty())
        filePath += ".gms";

    QFile file(filePath);
    if (!file.exists()) { // create
        file.open(QIODevice::WriteOnly);
        file.close();
    } else { // replace old
        file.resize(0);
    }
    QVector<ProjectGroupNode*> groups;
    for (ProjectAbstractNode *node: mNodes) {
        ProjectGroupNode *group = node->toGroup();
        if (!group) group = node->parentNode();
        if (!groups.contains(group))
            groups << group;
    }
    for (ProjectGroupNode *group: groups) {
        emit addExistingFile(group, filePath);
    }
}

void ProjectContextMenu::setParent(QWidget *parent)
{
    mParent = parent;
}

void ProjectContextMenu::onCloseGroup()
{
    for (ProjectAbstractNode *node: mNodes) {
        ProjectGroupNode *group = node->toGroup();
        if (!group) group = node->parentNode();
        if (group) emit closeGroup(group);
    }
}

//void ProjectContextMenu::onRunFile()
//{
//    ProjectFileNode *file = mNode->toFile();
//    if (file) emit runFile(file);
//}

void ProjectContextMenu::onSetMainFile()
{
    ProjectFileNode *file = mNodes.first()->toFile();
    if (file) emit setMainFile(file);
}

void ProjectContextMenu::onRenameGroup()
{
    ProjectGroupNode *group = mNodes.first()->toGroup();
    if (group) emit renameGroup(group);
}

void ProjectContextMenu::onCreateSolverOptionFile(const QString &solverName, const QString &solverOptionDefinitionFile)
{
    qDebug() << solverOptionDefinitionFile;

    QString filePath = QFileDialog::getSaveFileName(mParent,
                                                    QString("Create %1 option file...").arg(solverName),
                                                    QString("%1.opt").arg(solverName),
                                                    tr(QString("%1 option file (%1.opt %1.op*);;All files (*.*)").arg(solverName).toLatin1()),
                                                    nullptr,
                                                    DONT_RESOLVE_SYMLINKS_ON_MACOS);

    if (filePath == "") return;

    QFileInfo fi(filePath);
//    if (fi.suffix().isEmpty())
//        filePath += ".gms";

    QFile file(filePath);
    if (!file.exists()) { // create
        file.open(QIODevice::WriteOnly);
        file.close();
    } else { // replace old
        file.resize(0);
    }
    ProjectGroupNode *group = mNodes.first()->toGroup();
    if (!group) group = mNodes.first()->parentNode();
    emit newSolverOptionFile(group, solverOptionDefinitionFile, filePath);
}

void ProjectContextMenu::onOpenFileLoc()
{
    QString openLoc;
    ProjectFileNode *file = mNodes.first()->toFile();
    if (file) {
// select file on windows by calling explorer.exe with parameter /select
#ifdef _WIN32
        QString explorerPath = QStandardPaths::findExecutable("explorer.exe");
        if (explorerPath.isEmpty()) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(file->location()).path())));
        } else {
            QProcess proc;
            proc.setProgram(explorerPath);
            QStringList args;
            args << "/select";
            args << ",";
            args << QDir::toNativeSeparators(QFileInfo(file->location()).path()));
            proc.setArguments(args);
            proc.start();
            proc.waitForFinished();
        }
#else
        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(file->location()).path()));
#endif
    } else if ((mNodes.first()->type() == NodeType::group) || (mNodes.first()->type() == NodeType::runGroup)){
        ProjectGroupNode *group = mNodes.first()->toGroup();
        if (group) openLoc = group->location();
        QDesktopServices::openUrl(QUrl::fromLocalFile(openLoc));
    }
}

void ProjectContextMenu::onOpenLog()
{
    if (mNodes.first()) emit openLogFor(mNodes.first(), true);
}

} // namespace studio
} // namespace gams
