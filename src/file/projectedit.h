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
#ifndef GAMS_STUDIO_PROJECTEDIT_H
#define GAMS_STUDIO_PROJECTEDIT_H

#include "abstractview.h"
#include <QFrame>
#include <QLineEdit>
#include "common.h"

namespace gams {
namespace studio {
class PExProjectNode;

namespace project {

namespace Ui {
class ProjectEdit;
}

class ProjectData : public QObject
{
    Q_OBJECT
public:
    enum Field {file, name, nameExt, workDir, baseDir, mainGms};

    ProjectData(PExProjectNode *project);
    virtual ~ProjectData() override {}
    void setFieldData(Field field, QString value);
    QString fieldData(Field field);
    PExProjectNode *project() { return mProject; }
    void save();

signals:
    void changed(gams::studio::project::ProjectData::Field field);
    void tabNameChanged(gams::studio::PExProjectNode *project);

private slots:
    void projectChanged(gams::studio::NodeId id);

private:
    QHash<Field, QString> mData;
    PExProjectNode *mProject;
};


class ProjectEdit : public AbstractView
{
    Q_OBJECT

public:

    explicit ProjectEdit(ProjectData *sharedData, QWidget *parent = nullptr);
    ~ProjectEdit() override;
    ProjectData *sharedData() const;
    QString tabName(NameModifier mod = NameModifier::raw);
    bool isModified() const;
    void save();

signals:
    void modificationChanged(bool modification);

public slots:
    void updateData();

private slots:
    void on_edWorkDir_textChanged(const QString &text);
    void on_edBaseDir_textChanged(const QString &text);
    void on_bWorkDir_clicked();
    void on_bBaseDir_clicked();
    void projectChanged(gams::studio::NodeId id);

private:
    void setSharedData(ProjectData *sharedData);
    void updateEditColor(QLineEdit *edit, const QString &text);
    void updateState();
    void showDirDialog(const QString &title, QLineEdit *lineEdit, QString defaultDir);

    Ui::ProjectEdit *ui;
    ProjectData *mSharedData;
    bool mModified = false;
    QString mName;

};


} // namespace project
} // namespace studio
} // namespace gams
#endif // GAMS_STUDIO_PROJECTEDIT_H
