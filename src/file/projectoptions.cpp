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
#include "projectoptions.h"
#include "ui_projectoptions.h"
#include "file/pexgroupnode.h"
#include "file/filemeta.h"
#include "theme.h"

#include <QDir>
#include <QPushButton>
#include <QFont>
#include <QFileDialog>

namespace gams {
namespace studio {
namespace project {

ProjectData::ProjectData(PExProjectNode *project)
{
    mProject = project;
    if (!mData.contains(name)) mData.insert(name, project->name());
    if (!mData.contains(workDir)) mData.insert(workDir, QDir::toNativeSeparators(project->workDir()));
    if (!mData.contains(baseDir)) mData.insert(baseDir, QDir::toNativeSeparators(project->location()));
    if (!mData.contains(mainGms)) {
        if (mProject->runnableGms())
            mData.insert(mainGms, QDir::toNativeSeparators(mProject->runnableGms()->location()));
        else
            mData.insert(mainGms, "-no runnable-");

    }
    connect(project, &PExProjectNode::changed, this, &ProjectData::projectChanged);
}

void ProjectData::setFieldData(Field field, QString value)
{
    bool change = !mData.contains(field) || mData.value(field) != value;
    mData.insert(field, value);
    if (change) emit changed(field);
}

QString ProjectData::fieldData(Field field)
{
    return mData.value(field);
}

void ProjectData::save()
{
    if (mData.value(name).trimmed().compare(mProject->name()))
        mProject->setName(mData.value(name).trimmed());
    QString path = QDir::fromNativeSeparators(mData.value(baseDir)).trimmed();
    if (path.compare(mProject->location(), FileType::fsCaseSense()))
        mProject->setLocation(path);
    path = QDir::fromNativeSeparators(mData.value(workDir)).trimmed();
    if (path.compare(mProject->workDir(), FileType::fsCaseSense()))
        mProject->setWorkDir(path);
}

void ProjectData::projectChanged(NodeId id)
{
    if (mProject->id() != id) return;
    if (fieldData(ProjectData::name) != mProject->name())
        setFieldData(ProjectData::name, mProject->name());
    if (mProject->runnableGms())
        setFieldData(ProjectData::mainGms, QDir::toNativeSeparators(mProject->runnableGms()->location()));
    else
        setFieldData(ProjectData::mainGms, "-no runnable-");
}


ProjectOptions::ProjectOptions(ProjectData *sharedData,  QWidget *parent) :
    AbstractView(parent),
    ui(new Ui::ProjectOptions)
{
    ui->setupUi(this);
    mSharedData = sharedData;
    ui->edMainGms->setEnabled(false);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    ui->edBaseDir->setMinimumWidth(fontMetrics().height()*30);
    ui->edBaseDir->setToolTip("Base directory: used as base folder to represent the files");
    ui->edWorkDir->setToolTip("Working directory: used as working directory to run GAMS");
    ui->laBaseDir->setToolTip(ui->edBaseDir->toolTip());
    ui->laWorkDir->setToolTip(ui->edWorkDir->toolTip());
    ui->bBaseDir->setIcon(Theme::icon(":/%1/folder-open-bw"));
    ui->bWorkDir->setIcon(Theme::icon(":/%1/folder-open-bw"));
    adjustSize();
    connect(sharedData, &ProjectData::changed, this, &ProjectOptions::updateData);
    updateData();
}

ProjectOptions::~ProjectOptions()
{
    if (mSharedData->project()) mSharedData->project()->unlinkProjectOptionsFileMeta();
    delete ui;
}

ProjectData *ProjectOptions::sharedData() const
{
    return mSharedData;
}

bool ProjectOptions::isModified() const
{
    return mModified;
}

void ProjectOptions::save()
{
    mSharedData->save();
    updateState();
}

void ProjectOptions::on_edName_textChanged(const QString &text)
{
    if (text != mSharedData->fieldData(ProjectData::name))
        mSharedData->setFieldData(ProjectData::name, text);
    updateState();
}

void ProjectOptions::on_edWorkDir_textChanged(const QString &text)
{
    updateEditColor(ui->edWorkDir, text);
    if (text != mSharedData->fieldData(ProjectData::workDir))
        mSharedData->setFieldData(ProjectData::workDir, text);
    updateState();
}

void ProjectOptions::on_edBaseDir_textChanged(const QString &text)
{
    updateEditColor(ui->edBaseDir, text);
    if (text != mSharedData->fieldData(ProjectData::baseDir))
        mSharedData->setFieldData(ProjectData::baseDir, text);
    updateState();
}

void ProjectOptions::updateEditColor(QLineEdit *edit, const QString &text)
{
    QDir dir(text.trimmed());
    if (!dir.exists()) {
        QPalette pal = edit->palette();
        pal.setColor(QPalette::Text, Theme::color(Theme::Mark_errorFg));
        edit->setPalette(pal);
    } else {
        edit->setPalette(QPalette());
    }
}

void ProjectOptions::updateState()
{
    bool isModified = false;
    if (ui->edName->text().trimmed().compare(mSharedData->project()->name()))
        isModified = true;
    QString path = QDir::fromNativeSeparators(ui->edBaseDir->text()).trimmed();
    if (path.compare(mSharedData->project()->location(), FileType::fsCaseSense()))
        isModified = true;
    path = QDir::fromNativeSeparators(ui->edWorkDir->text()).trimmed();
    if (path.compare(mSharedData->project()->workDir(), FileType::fsCaseSense()))
        isModified = true;
    if (isModified != mModified) {
        mModified = isModified;
        emit modificationChanged(mModified);
    }
}

void ProjectOptions::on_bWorkDir_clicked()
{
    showDirDialog("Select Working Directory", ui->edWorkDir, mSharedData->project()->workDir());
}

void ProjectOptions::on_bBaseDir_clicked()
{
    showDirDialog("Select Base Directory", ui->edBaseDir, mSharedData->project()->location());
}

void ProjectOptions::projectChanged(NodeId id)
{
    if (mSharedData->project()->id() != id) return;
    if (mSharedData->fieldData(ProjectData::name) != mSharedData->project()->name()) {
        mSharedData->setFieldData(ProjectData::name, mSharedData->project()->name());
    }
    if (mSharedData->project()->runnableGms())
        ui->edMainGms->setText(QDir::toNativeSeparators(mSharedData->project()->runnableGms()->location()));
    else
        ui->edMainGms->setText("-no runnable-");
    updateState();
}

void ProjectOptions::updateData()
{
    if (ui->edName->text() != mSharedData->fieldData(ProjectData::name))
        ui->edName->setText(mSharedData->fieldData(ProjectData::name));
    if (ui->edWorkDir->text() != mSharedData->fieldData(ProjectData::workDir))
        ui->edWorkDir->setText(mSharedData->fieldData(ProjectData::workDir));
    if (ui->edBaseDir->text() != mSharedData->fieldData(ProjectData::baseDir))
        ui->edBaseDir->setText(mSharedData->fieldData(ProjectData::baseDir));
    if (ui->edMainGms->text() != mSharedData->fieldData(ProjectData::mainGms))
        ui->edMainGms->setText(mSharedData->fieldData(ProjectData::mainGms));
}

void ProjectOptions::showDirDialog(const QString &title, QLineEdit *lineEdit, QString defaultDir)
{
    QString path = QDir::fromNativeSeparators(ui->edBaseDir->text()).trimmed();
    QDir dir(path);
    if (!dir.exists()) path = defaultDir;
    QFileDialog *dialog = new QFileDialog(this, title, path);
    dialog->setFileMode(QFileDialog::Directory);
    connect(dialog, &QFileDialog::accepted, this, [lineEdit, dialog]() {
        if (dialog->selectedFiles().count() == 1) {
            QDir dir(dialog->selectedFiles().first().trimmed());
            if (dir.exists()) lineEdit->setText(QDir::toNativeSeparators(dir.path()));
        }
    });
    connect(dialog, &QFileDialog::finished, this, [dialog]() { dialog->deleteLater(); });
    dialog->setModal(true);
    dialog->open();
}

} // namespace project
} // namespace studio
} // namespace gams
