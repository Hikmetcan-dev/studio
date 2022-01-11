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
#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include "file/filemeta.h"
#include "searchresultmodel.h"

namespace gams {
namespace studio {
namespace search {

class SearchDialog;
class Search : public QObject
{
    Q_OBJECT
public:
    enum Scope {
        ThisFile = 0,
        ThisProject= 1,
        Selection= 2,
        OpenTabs = 3,
        AllFiles = 4
    };

    enum Status {
        Searching = 0,
        NoResults = 1,
        Clear = 2,
        Replacing = 4
    };

    enum Direction {
        Forward = 0,
        Backward = 1
    };
    Search(SearchDialog *sd);

    void setParameters(QList<FileMeta*> files, QRegularExpression regex, bool searchBackwards = false);
    void start();
    void stop();

    void findNext(Direction direction);
    void replaceNext(QString replacementText);
    void replaceAll(QString replacementText);
    void selectNextMatch(Direction direction = Direction::Forward, bool firstLevel = true);

    bool isRunning() const;
    QList<Result> results() const;
    QList<Result> filteredResultList(QString fileLocation);
    const QFlags<QTextDocument::FindFlag> &options() const;
    QRegularExpression regex() const;
    Search::Scope scope() const;
    bool hasSearchSelection();
    void reset();
    void documentChanged();
    void resetResults();

signals:
    void updateLabelByCursorPos(int line, int col);
    void invalidateResults();
    void selectResult(int matchNr);

private:
    void findInDoc(FileMeta* fm);
    void findInSelection();
    void findOnDisk(QRegularExpression searchRegex, FileMeta *fm, SearchResultModel* collection);

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
    QList<FileMeta*> mFiles;
    QRegularExpression mRegex;
    QFlags<QTextDocument::FindFlag> mOptions;
    Scope mScope;

    FileId mSearchSelectionFile;

    QThread mThread;
    bool mSearching = false;
    bool mCacheAvailable = false;
    bool mOutsideOfList = false;
    int mLastMatchInOpt = -1;

    bool mSplitSearchContinue = false;
};

}
}
}

#endif // SEARCH_H
