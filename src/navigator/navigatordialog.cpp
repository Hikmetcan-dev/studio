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
 */
#include <QKeyEvent>
#include <QStatusBar>
#include "ui_navigatordialog.h"
#include "navigator/navigatorlineedit.h"
#include "navigatordialog.h"

namespace gams {
namespace studio {

NavigatorDialog::NavigatorDialog(MainWindow *main, NavigatorLineEdit* inputField)
    : QDialog((QWidget*)main), ui(new Ui::NavigatorDialog), mMain(main), mInput(inputField)
{
    setWindowFlags(Qt::ToolTip);
    setFocusProxy(mInput);

    ui->setupUi(this);
    mNavModel = new NavigatorModel(this, main);
    mFilterModel = new QSortFilterProxyModel(this);

    mFilterModel->setSourceModel(mNavModel);
    mFilterModel->sort(0);
    mFilterModel->setDynamicSortFilter(false);
    mFilterModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    mFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    mFilterModel->setFilterKeyColumn(0);

    ui->tableView->setModel(mFilterModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->installEventFilter(this);

    connect(mInput, &QLineEdit::returnPressed, this, &NavigatorDialog::returnPressed);
    connect(mInput, &QLineEdit::textEdited, this, &NavigatorDialog::setInput);
}

NavigatorDialog::~NavigatorDialog()
{
    delete ui;
    delete mNavModel;
    delete mFilterModel;
}

void NavigatorDialog::showEvent(QShowEvent *e)
{
    Q_UNUSED(e)

    updatePosition();
    setInput(mMain->navigatorInput()->text());
}

void NavigatorDialog::setInput(const QString &input)
{
    if (!isVisible())
        show();

    QString filter = input;
    NavigatorMode mode;
    if (input.startsWith("?")) {
        mode = NavigatorMode::Help;
        mFilterModel->setFilterWildcard("");
    } else if (input.startsWith(":")) {
        mode = NavigatorMode::Line;
        mFilterModel->setFilterWildcard("");
    } else if (input.startsWith("p ")) {
        mode = NavigatorMode::InProject;
        mFilterModel->setFilterWildcard(filter.remove(0, 2));
    } else if (input.startsWith("t ")) {
        mode = NavigatorMode::Tabs;
        mFilterModel->setFilterWildcard(filter.remove(0, 2));
    } else if (input.startsWith("l ")) {
        mode = NavigatorMode::Logs;
        mFilterModel->setFilterWildcard(filter.remove(0, 2));
    } else if (input.startsWith("f ")) {
        mode = NavigatorMode::Folder;
    } else {
        mode = NavigatorMode::AllFiles;
        mFilterModel->setFilterWildcard(input);
    }
    updateContent(mode);
}

void NavigatorDialog::updateContent(NavigatorMode mode) {
    QVector<NavigatorContent> content = QVector<NavigatorContent>();
    switch (mode) {
        case NavigatorMode::Help:
            generateHelpContent(content);
        break;
        case NavigatorMode::Line:
            navigateLine(content);
        break;
        case NavigatorMode::AllFiles:
            collectAllFiles(content);
        break;
        case NavigatorMode::InProject:
            collectInProject(content);
        break;
        case NavigatorMode::Tabs:
            collectTabs(content);
        break;
        case NavigatorMode::Logs:
            collectLogs(content);
        break;
        case NavigatorMode::Folder:
            collectFileSystem(content);
        break;
        default:
            qWarning() << "Unhandled NavigatorMode";
        break;
    }
    mNavModel->setContent(content);
    mCurrentMode = mode;
    if (mode != NavigatorMode::Folder) {
        mNavModel->setCurrentDir(QDir(mMain->recent()->path()));
        mDirSelectionOngoing = false;
    }

    // select first entry if user hasnt anything selected
    if (mFilterModel && !ui->tableView->selectionModel()->hasSelection())
        ui->tableView->setCurrentIndex(mFilterModel->index(0, 0));
}

void NavigatorDialog::generateHelpContent(QVector<NavigatorContent> &content)
{
    content.append(NavigatorContent(":number", "jump to line number"));
    content.append(NavigatorContent("filename", "filter all files"));
    content.append(NavigatorContent("P filename", "filter files in current project"));
    content.append(NavigatorContent("T filename", "filter open tabs"));
    content.append(NavigatorContent("L filename", "filter logs"));
}

void NavigatorDialog::collectAllFiles(QVector<NavigatorContent> &content)
{
    collectTabs(content);
    collectInProject(content);
    collectLogs(content);

    foreach (FileMeta* fm, mMain->fileRepo()->fileMetas()) {
        if (!valueExists(fm, content) && !fm->location().endsWith("~log")) {
            content.append(NavigatorContent(fm, "Known Files"));
        }
    }
}

void NavigatorDialog::collectInProject(QVector<NavigatorContent> &content)
{
    PExFileNode* currentFile = mMain->projectRepo()->findFileNode(mMain->currentEdit());
    if (!currentFile) return;

    for (PExFileNode* f : currentFile->assignedProject()->listFiles()) {
        FileMeta* fm = f->file();
        if (!valueExists(fm, content)) {
            content.append(NavigatorContent(fm, "Current Project"));
        }
    }
}

void NavigatorDialog::collectTabs(QVector<NavigatorContent> &content)
{
    foreach (FileMeta* fm, mMain->fileRepo()->openFiles()) {
        if (!valueExists(fm, content) && !fm->location().endsWith("~log")) {
            content.append(NavigatorContent(fm, "Open Tabs"));
        }
    }
}

void NavigatorDialog::collectLogs(QVector<NavigatorContent> &content)
{
    foreach (PExProjectNode* project, mMain->projectRepo()->projects()) {
        PExLogNode* log = project->logNode();

        FileMeta* fm = log->file();
        if (fm->editors().empty()) continue;

        content.append(NavigatorContent(fm, "Open Logs"));
    }
}

void NavigatorDialog::collectFileSystem(QVector<NavigatorContent> &content)
{
    QString textInput = mMain->navigatorInput()->text();
    textInput = textInput.remove(0, 2); // remove prefix

    QDir dir(textInput);
    if (!mDirSelectionOngoing) {
        mSelectedDirectory = mNavModel->currentDir();
        mDirSelectionOngoing = true;
    } else if (dir.exists()) {
        if (dir.isRelative()) {
            mSelectedDirectory.setPath(mNavModel->currentDir().absolutePath() + QDir::separator() + textInput);
        } else mSelectedDirectory = dir;

        textInput.append(QDir::separator()); // we need paths to end in seperator for better usability
    } else {
        mSelectedDirectory = findClosestPath(textInput);
    }

    // filter prefix and extract relevant wildcard term
    QString filter = textInput.right(textInput.length() - textInput.lastIndexOf(QDir::separator()));
    filter.remove(QDir::separator());
    mFilterModel->setFilterWildcard(filter);

    for (const QFileInfo &entry : mSelectedDirectory.entryInfoList(QDir::NoDot|QDir::AllEntries, QDir::Name|QDir::DirsFirst)) {
        content.append(NavigatorContent(entry, entry.isDir() ? "Directory" : "File"));
    }
}

void NavigatorDialog::navigateLine(QVector<NavigatorContent> &content)
{
    FileMeta* fm = mMain->fileRepo()->fileMeta(mMain->recent()->editor());
    content.append(NavigatorContent(fm, "Max Lines: " + QString::number(mMain->linesInCurrentEditor())));
}

void NavigatorDialog::returnPressed()
{
    QModelIndex index = mFilterModel->mapToSource(ui->tableView->currentIndex());

    if (mCurrentMode == NavigatorMode::Line) {
        QString inputText = mInput->text();
        bool ok = false;

        int lineNr = inputText.midRef(1).toInt(&ok);
        if (ok) mMain->jumpToLine(lineNr-1);
        close();

    } else if (index.row() != -1) {
        selectFileOrFolder(index);
    } else {
        close();
    }
}

void NavigatorDialog::selectFileOrFolder(QModelIndex index)
{
    NavigatorContent nc = mNavModel->content().at(index.row());
    if (FileMeta* fm = nc.fileMeta) {
        if (fm->location().endsWith("~log"))
            mMain->jumpToTab(fm);
        else mMain->openFile(fm, true);

        close();
    } else {
        if (nc.fileInfo.isFile()) {
            mMain->openFilePath(nc.fileInfo.absoluteFilePath());
            close();
        } else {
            mSelectedDirectory = QDir(nc.fileInfo.absoluteFilePath());
            mMain->navigatorInput()->setText(
                        "f " + mSelectedDirectory.absolutePath() + QDir::separator());
            updateContent(NavigatorMode::Folder);
        }
    }
}

void NavigatorDialog::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Down) {
        int pos = ui->tableView->currentIndex().row() + 1;
        if (pos >= ui->tableView->model()->rowCount())
            pos = 0;

        ui->tableView->setCurrentIndex(mFilterModel->index(pos, 0));
    } else if (e->key() == Qt::Key_Up) {
        int pos = ui->tableView->currentIndex().row() - 1;
        if (pos < 0)
            pos = ui->tableView->model()->rowCount() - 1;

        ui->tableView->setCurrentIndex(mFilterModel->index(pos, 0));
    } else if (e->key() == Qt::Key_Escape) {
        close();
    }
}

void NavigatorDialog::updatePosition()
{
    QPoint position;

    position.setX(qMin(mInput->pos().x(), mMain->width() - width()));
    position.setY(mMain->height() - height() - mMain->statusBar()->height());

    move(mMain->mapToGlobal(position));
}

void NavigatorDialog::receiveKeyEvent(QKeyEvent *event)
{
    keyPressEvent(event);
}

bool NavigatorDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != ui->tableView) return false;
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down) {
            keyPressEvent(keyEvent);
            return true;
        }
    }
    return false;
}

bool NavigatorDialog::valueExists(FileMeta* fm, const QVector<NavigatorContent>& content)
{
    foreach (NavigatorContent c, content) {
        if (c.fileMeta == fm)
            return true;
    }
    return false;
}

QDir NavigatorDialog::findClosestPath(const QString& path)
{
    QString tryPath = path;

    int i = 10;
    while(tryPath.length() > 0) {
        tryPath = tryPath.left(tryPath.length()-1);
        if (tryPath.isEmpty()) tryPath = mNavModel->currentDir().absolutePath();

        QDir d(tryPath);
        if (d.exists()) {
            return d;
        }

        // infinite loop safeguard
        if (i-- == 0)
            break;
    }
    return QDir();
}

}
}
