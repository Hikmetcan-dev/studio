/*
 * This file is part of the GAMS Studio project.
 *
 * Copyright (c) 2017-2021 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2021 GAMS Development Corp. <support@gams.com>
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
#ifndef PEXGROUPNODE_H
#define PEXGROUPNODE_H

#include <QProcess>
#include <QFileInfoList>
#include <memory>
#include "process/abstractprocess.h"
#include "editors/logparser.h"
#include "pexabstractnode.h"
#include "syntax/textmark.h"

namespace gams {
namespace studio {

class PExLogNode;
class PExFileNode;
class TextMarkRepo;
class FileMeta;
class FileMetaRepo;
namespace option {
struct OptionItem;
class Option;
}

class PExGroupNode : public PExAbstractNode
{
    Q_OBJECT
public:
    virtual ~PExGroupNode() override;

    QIcon icon(QIcon::Mode mode = QIcon::Normal, int alpha = 100) override;
    int childCount() const;
    bool isEmpty();
    PExAbstractNode* childNode(int index) const;
    int indexOf(PExAbstractNode *child);
    virtual QString location() const;
    QString tooltip() override;
    virtual QString errorText(int lstLine);
    PExFileNode *findFile(QString location, bool recurse = true) const;
    PExFileNode *findFile(const FileMeta *fileMeta, bool recurse = true) const;
    QList<PExFileNode*> findFiles(FileKind kind, bool recurse) const;
    PExProjectNode *findProject(const AbstractProcess *process) const;
    PExProjectNode *findProject(FileId runId) const;
    QVector<PExFileNode*> listFiles(bool recurse = false) const;
    void moveChildNode(int from, int to);
    const QList<PExAbstractNode*> &childNodes() const { return mChildNodes; }

public slots:
    void hasFile(QString fName, bool &exists);

protected:
    friend class ProjectRepo;
    friend class PExAbstractNode;
    friend class PExLogNode;
    friend class PExFileNode;

    PExGroupNode(QString name, QString location, NodeType type = NodeType::group);
    virtual void appendChild(PExAbstractNode *child);
    virtual void removeChild(PExAbstractNode *child);
    void setLocation(const QString &location);

private:
    QList<PExAbstractNode*> mChildNodes;
    QString mLocation;
};

class PExProjectNode : public PExGroupNode
{
    Q_OBJECT
public:
    QIcon icon(QIcon::Mode mode = QIcon::Normal, int alpha = 100) override;
    bool hasLogNode() const;
    PExLogNode* logNode();
    FileMeta *runnableGms() const;
    void setRunnableGms(FileMeta *gmsFile = nullptr);
    QString mainModelName(bool stripped = true) const;
    QString tooltip() override;
    QString errorText(int lstLine) override;
    void clearErrorTexts();
    bool hasErrorText(int lstLine = -1);
    void addRunParametersHistory(QString option);
    QStringList getRunParametersHistory() const;
    QStringList analyzeParameters(const QString &gmsLocation, QStringList defaultParameters, QList<option::OptionItem> itemList, option::Option *opt = nullptr);

    QString parameter(const QString& kind) const;
    bool hasParameter(const QString& kind) const;
    void addNodesForSpecialFiles();
    void setParameter(const QString& kind, const QString& path);
    void clearParameters();

    bool isProcess(const AbstractProcess *process) const;
    QProcess::ProcessState gamsProcessState() const;
    void setProcess(std::unique_ptr<AbstractProcess> process);
    AbstractProcess *process() const;
    bool jumpToFirstError(bool focus, PExFileNode *lstNode);

signals:
    void gamsProcessStateChanged(PExGroupNode* group);
    void getParameterValue(QString param, QString &value);

public slots:
    void setErrorText(int lstLine, QString text);
    void hasHRef(const QString &href, QString &fileName);
    void jumpToHRef(const QString &href);
    void createMarks(const LogParser::MarkData &marks);
    void switchLst(const QString &lstFile);
    void registerGeneratedFile(const QString &fileName);

protected slots:
    void onGamsProcessStateChanged(QProcess::ProcessState newState);

protected:
    friend class ProjectRepo;
    friend class PExAbstractNode;
    friend class PExLogNode;
    friend class PExFileNode;

    PExProjectNode(QString name, QString path, FileMeta *runFileMeta = nullptr);
    void errorTexts(const QVector<int> &lstLines, QStringList &result);
    void setLogNode(PExLogNode* logNode);
    void appendChild(PExAbstractNode *child) override;
    void removeChild(PExAbstractNode *child) override;
    QString resolveHRef(QString href, PExFileNode *&node, int &line, int &col, bool create = false);

private:
    std::unique_ptr<AbstractProcess> mGamsProcess;
    PExLogNode* mLogNode = nullptr;
    QHash<int, QString> mErrorTexts;
    QStringList mRunParametersHistory;
    QHash<QString, QString> mParameterHash;

private:
    QString cleanPath(QString path, QString file);
    void setLogLocation(QString path);
};


class ProjectRootNode : public PExGroupNode
{
    Q_OBJECT
public:
    ProjectRootNode(ProjectRepo *projectRepo);
    ~ProjectRootNode() override {}
    ProjectRepo *projectRepo() const override;
    FileMetaRepo *fileRepo() const override;
    TextMarkRepo *textMarkRepo() const override;

private:
    friend class ProjectRepo;
    void setParentNode(PExGroupNode *parent) override;
    void init(ProjectRepo* projectRepo);

private:
    ProjectRepo* mRepo = nullptr;
};

} // namespace studio
} // namespace gams

#endif // PEXGROUPNODE_H