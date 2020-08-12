#include "gamsinstprocess.h"

namespace gams {
namespace studio {
namespace process {

GamsInstProcess::GamsInstProcess(QObject *parent)
    : AbstractGamsProcess("gamsinst", parent)
{
    setParameters(QStringList() << "-listdirs");
    connect(this, &GamsInstProcess::newStdChannelData, this, &GamsInstProcess::newData);
}

void GamsInstProcess::execute()
{
#if defined(__unix__) || defined(__APPLE__)
    emit newProcessCall("Running:", appCall(nativeAppPath(), parameters()));
    mProcess.start(nativeAppPath(), parameters());
#else
    mProcess.setNativeArguments(parameters().join(" "));
    mProcess.setProgram(nativeAppPath());
    emit newProcessCall("Running:", appCall(nativeAppPath(), parameters()));
    mProcess.start();
#endif
}

void GamsInstProcess::newData(const QByteArray &data)
{
    if (data.startsWith("Config")) isData = false;
    else if (data.startsWith("Data")) isData = true;
    else {
        QString line = data.trimmed();
        if (line.isEmpty()) return;
        if (isData) mData << line;
        else mConfig << line;
    }
}

} // namespace process
} // namespace studio
} // namespace gams
