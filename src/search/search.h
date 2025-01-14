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
#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include "file/filemeta.h"
#include "searchresultmodel.h"

namespace gams {
namespace studio {
namespace search {

class SearchDialog;
class AbstractSearchFileHandler;
class Search : public QObject
{
    Q_OBJECT
public:
    enum Scope {
        Selection,
        ThisFile,
        ThisProject,
        OpenTabs,
        AllFiles,
        Folder
    };

    enum Status {
        Searching,
        NoResults,
        Clear,
        Replacing,
        InvalidPath,
        EmptySearchTerm,
        CollectingFiles,
        InvalidRegex,
        Ok
    };

    enum Direction {
        Forward,
        Backward
    };
    Search(SearchDialog *sd, AbstractSearchFileHandler *fileHandler);

    void start(bool ignoreReadonly, bool searchBackwards, bool showResults);
    void stop();

    void findNext(Direction direction);
    void replaceNext(QString replacementText);
    void replaceAll(QString replacementText);
    void selectNextMatch(Direction direction = Direction::Forward, bool firstLevel = true);

    bool isSearching() const;
    QList<Result> results() const;
    QList<Result> filteredResultList(QString fileLocation);
    const QFlags<QTextDocument::FindFlag> &options() const;
    const QRegularExpression regex() const;
    Search::Scope scope() const;
    bool hasSearchSelection();
    void reset();
    void invalidateCache();
    void resetResults();
    void activeFileChanged();
    bool hasCache() const;

signals:
    void invalidateResults();
    void updateUI();
    void selectResult(int matchNr);

private:
    void findInDoc(FileMeta* fm);
    void findInSelection(bool showResults);
    void findOnDisk(QRegularExpression searchRegex, FileMeta *fm, SearchResultModel* collection);
    void setParameters(bool ignoreReadonly, bool searchBackwards = false);

    int replaceOpened(FileMeta* fm, QRegularExpression regex, QString replaceTerm);
    int replaceUnopened(FileMeta* fm, QRegularExpression regex, QString replaceTerm);

    QPair<int, int> cursorPosition();
    int findNextEntryInCache(Search::Direction direction);
    void jumpToResult(int matchNr);

    int NavigateOutsideCache(Direction direction, bool firstLevel);
    int NavigateInsideCache(Direction direction);

    void checkFileChanged(FileId fileId);
    bool hasResultsForFile(QString filePath);

private slots:
    void finished();

private:
    SearchDialog* mSearchDialog;
    QList<Result> mResults;
    QHash<QString, QList<Result>> mResultHash;
    QSet<FileMeta*> mFiles;
    QRegularExpression mRegex;
    QString mSearchTerm;
    QFlags<QTextDocument::FindFlag> mOptions;
    Scope mScope;
    AbstractSearchFileHandler* mFileHandler;
    FileId mSearchSelectionFile;
    QThread mThread;

    bool mSearching = false;
    bool mJumpQueued = false;
    bool mCacheAvailable = false;
    bool mOutsideOfList = false;
    int mLastMatchInOpt = -1;

    bool mSplitSearchContinue = false;
};

}
}
}

#endif // SEARCH_H
