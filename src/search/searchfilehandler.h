#ifndef SEARCHFILEHANDLER_H
#define SEARCHFILEHANDLER_H

#include "search/abstractsearchfilehandler.h"
#include "mainwindow.h"

class MainWindow;
namespace gams {
namespace studio {
namespace search {

class SearchFileHandler : public AbstractSearchFileHandler
{
public:
    SearchFileHandler(MainWindow* main);

    FileMeta* fileMeta(QWidget* widget) override;
    FileMeta* fileMeta(FileId fileId) override;
    QList<FileMeta*> fileMetas() override;
    QList<FileMeta*> openFiles() override;
    PExFileNode* fileNode(QWidget* widget) override;
    PExFileNode* findFile(QString filepath) override;

private:
    MainWindow* mMain;
};

}
}
}

#endif // SEARCHFILEHANDLER_H