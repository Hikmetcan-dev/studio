#ifndef AUTOSAVEHANDLER_H
#define AUTOSAVEHANDLER_H

#include <QStringList>
#include <QWidget>

namespace gams {
namespace studio {

class MainWindow;

class AutosaveHandler
{
public:
    AutosaveHandler(MainWindow *mainWindow);

    QStringList checkForAutosaveFiles();

    void recoverAutosaveFiles(const QStringList &autosaveFiles);

    void saveChangedFiles();

private:
    MainWindow *mMainWindow;
    const QString mAutosavedFileMarker = "~$";
};

} // namespace studio
} // namespace gams

#endif // AUTOSAVEHANDLER_H
