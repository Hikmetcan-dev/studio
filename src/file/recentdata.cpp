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
#include "recentdata.h"
#include "mainwindow.h"
#include "settings.h"
#include "viewhelper.h"

namespace gams {
namespace studio {

void RecentData::setEditor(QWidget *edit, MainWindow* window)
{
    if (mEditor == edit) return;

    if (AbstractEdit* aEdit = ViewHelper::toAbstractEdit(mEditor)) {
        MainWindow::disconnect(aEdit, &AbstractEdit::cursorPositionChanged, window, &MainWindow::updateEditorPos);
        MainWindow::disconnect(aEdit, &AbstractEdit::selectionChanged, window, &MainWindow::updateEditorPos);
        MainWindow::disconnect(aEdit, &AbstractEdit::blockCountChanged, window, &MainWindow::updateEditorBlockCount);
        MainWindow::disconnect(aEdit->document(), &QTextDocument::contentsChange, window, &MainWindow::currentDocumentChanged);
    } else if (option::SolverOptionWidget* soEdit = ViewHelper::toSolverOptionEdit(mEditor)) {
        MainWindow::disconnect(soEdit, &option::SolverOptionWidget::itemCountChanged, window, &MainWindow::updateEditorItemCount );
    }
    if (TextView* tv = ViewHelper::toTextView(mEditor)) {
//        MainWindow::disconnect(tv, &TextView::cursorPositionChanged, window, &MainWindow::updateEditorPos);
        MainWindow::disconnect(tv, &TextView::selectionChanged, window, &MainWindow::updateEditorPos);
        MainWindow::disconnect(tv, &TextView::blockCountChanged, window, &MainWindow::updateEditorBlockCount);
        MainWindow::disconnect(tv, &TextView::loadAmountChanged, window, &MainWindow::updateLoadAmount);
        window->resetLoadAmount();
    }

    mEditor = edit;
    mProject = (mEditor ?  window->projectRepo()->asProject(ViewHelper::groupId(mEditor)) : nullptr);

    if (FileMeta *fm = window->fileRepo()->fileMeta(mEditor)) {
        if (fm->kind() != FileKind::PrO)
            mPersistentEditor = mEditor;
    }
    if (PExFileNode* node = window->projectRepo()->findFileNode(mEditor)) {
        mEditFileId = node->file()->id();
        mPath = QFileInfo(node->location()).path();
    } else {
        mEditFileId = ViewHelper::fileId(mEditor);
        mPath = Settings::settings()->toString(skDefaultWorkspace);
    }

    if (AbstractEdit* aEdit = ViewHelper::toAbstractEdit(mEditor)) {
        MainWindow::connect(aEdit, &AbstractEdit::cursorPositionChanged, window, &MainWindow::updateEditorPos);
        MainWindow::connect(aEdit, &AbstractEdit::selectionChanged, window, &MainWindow::updateEditorPos);
        MainWindow::connect(aEdit, &AbstractEdit::blockCountChanged, window, &MainWindow::updateEditorBlockCount);
        MainWindow::connect(aEdit->document(), &QTextDocument::contentsChange, window, &MainWindow::currentDocumentChanged);
    } else if (option::SolverOptionWidget* soEdit = ViewHelper::toSolverOptionEdit(mEditor)) {
        MainWindow::connect(soEdit, &option::SolverOptionWidget::itemCountChanged, window, &MainWindow::updateEditorItemCount );
    }
    if (TextView* tv = ViewHelper::toTextView(mEditor)) {
        MainWindow::connect(tv, &TextView::selectionChanged, window, &MainWindow::updateEditorPos, Qt::UniqueConnection);
//        MainWindow::connect(tv, &TextView::cursorPositionChanged, window, &MainWindow::updateEditorPos);
        MainWindow::connect(tv, &TextView::blockCountChanged, window, &MainWindow::updateEditorBlockCount, Qt::UniqueConnection);
        MainWindow::connect(tv, &TextView::loadAmountChanged, window, &MainWindow::updateLoadAmount, Qt::UniqueConnection);
    }
    window->updateEditorMode();
    window->updateEditorPos();
}

void RecentData::reset()
{
    mEditFileId = -1;
    mPath = Settings::settings()->toString(skDefaultWorkspace);
    mProject = nullptr;
    mEditor = nullptr;
    mPersistentEditor = nullptr;
}


} // namespace studio
} // namespace gams
