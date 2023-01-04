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
#include "commonpaths.h"
#include "exception.h"

#ifdef __APPLE__
#include "../platform/macos/macospathfinder.h"
#include "../platform/macos/macoscocoabridge.h"
#endif

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>

namespace gams {
namespace studio {

#if defined(__APPLE__)
    const QString CommonPaths::DocumentationDir = "../../../Resources/docs";
    const QString CommonPaths::ModlibsPrefixPath = "../../../Resources/";
#else
    const QString CommonPaths::DocumentationDir = "docs";
    const QString CommonPaths::ModlibsPrefixPath = "";
#endif

QString CommonPaths::SystemDir = QString();
QStringList CommonPaths::GamsStandardConfigPaths;
QStringList CommonPaths::GamsStandardDataPaths;

#if defined(__APPLE__) || defined(__unix__)
    const QString CommonPaths::ConfigFile = "gmscmpun.txt";
    const QString CommonPaths::UserLicensePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    const QString CommonPaths::GamsConfigPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)  + "/GAMS";

#else
    const QString CommonPaths::ConfigFile = "gmscmpnt.txt";
    const QString CommonPaths::UserLicensePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    const QString CommonPaths::GamsConfigPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/GAMS";
#endif

const QString CommonPaths::GamsUserConfigFile = "gamsconfig.yaml";
const QString CommonPaths::LicenseFile = "gamslice.txt";

CommonPaths::CommonPaths()
{

}

const QString& CommonPaths::documentationDir()
{
    return DocumentationDir;
}

QString CommonPaths::modelLibraryDir(const QString &libname)
{
    return (ModlibsPrefixPath.isEmpty() ? libname : ModlibsPrefixPath+libname);
}

const QString& CommonPaths::systemDir()
{
    return SystemDir;
}

void CommonPaths::setSystemDir(const QString &sysdir)
{
    if (!sysdir.isEmpty()) {
        auto path = QFileInfo(QStandardPaths::findExecutable("gams", {sysdir})).absolutePath();
        SystemDir = QDir::cleanPath(path);
        return;
    }

#ifdef __APPLE__
    SystemDir = MacOSPathFinder::systemDir();
#else
    QString gamsPath;
    const QString subPath = QString(QDir::separator()).append("..");
#ifdef __unix__
    QFileInfo fileInfo(qgetenv("APPIMAGE"));
    gamsPath = fileInfo.absoluteDir().path().append(subPath);
#else
    gamsPath = QCoreApplication::applicationDirPath().append(subPath);
#endif

    QString path = QStandardPaths::findExecutable("gams", {gamsPath});
    if (path.isEmpty()) {
        gamsPath = QFileInfo(QStandardPaths::findExecutable("gams")).absolutePath();
    }

    SystemDir = QDir::cleanPath(gamsPath);
#endif
}

bool CommonPaths::isSystemDirValid()
{
    return !QStandardPaths::findExecutable("gams", {SystemDir}).isEmpty();
}

QString CommonPaths::gamsDocumentsDir()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (dir.isEmpty())
        FATAL() << "Unable to access user documents location";
    QDir gamsDocumentsDir = QDir::cleanPath(dir + "/GAMS");
    if(!gamsDocumentsDir.exists())
        gamsDocumentsDir.mkpath(".");
    return gamsDocumentsDir.path();
}

QString CommonPaths::userDocumentsDir()
{
    QDir userDocumentsDir(gamsDocumentsDir() + "/Studio");
    if (!userDocumentsDir.exists())
        userDocumentsDir.mkpath(".");
    return userDocumentsDir.path();
}

QString CommonPaths::userModelLibraryDir()
{
    QDir userModelLibraryDir(gamsDocumentsDir() + "/modellibs");
    if(!userModelLibraryDir.exists())
        userModelLibraryDir.mkpath(".");
    return userModelLibraryDir.path();
}

QString CommonPaths::gamsLicenseFilePath(const QStringList &dataPaths)
{
    Q_FOREACH(const auto &path, dataPaths) {
        auto filePath = path + "/" +LicenseFile;
        if (QFileInfo::exists(filePath))
            return QDir::cleanPath(filePath);
    }
    if (!dataPaths.isEmpty())
        return QDir::cleanPath(dataPaths.first() + "/" + LicenseFile);
    return QDir::cleanPath(systemDir() + "/" + LicenseFile);
}

QString CommonPaths::gamsUserConfigDir()
{
    QDir gamsUserConfigDir = QDir(GamsConfigPath);
    if(!gamsUserConfigDir.exists())
        gamsUserConfigDir.mkpath(".");
    return gamsUserConfigDir.path();
}

QString CommonPaths::defaultGamsUserConfigFile()
{
    return QDir::cleanPath(gamsUserConfigDir() + "/" + GamsUserConfigFile);
}

void CommonPaths::setGamsStandardPaths(QStringList gamsPaths, StandardPathType pathType)
{
    switch (pathType) {
    case StandardConfigPath: GamsStandardConfigPaths = gamsPaths; break;
    case StandardDataPath: GamsStandardDataPaths = gamsPaths; break;
    default:
        EXCEPT() << "Error in setGamsStandardPaths: pathType must be GamsStandardConfigPaths or GamsStandardDataPaths";
    }
}

QStringList CommonPaths::gamsStandardPaths(StandardPathType pathType)
{
    switch (pathType) {
    case StandardConfigPath: return GamsStandardConfigPaths;
    case StandardDataPath: return GamsStandardDataPaths;
    default: {
        QStringList res = GamsStandardConfigPaths;
        Q_FOREACH (const QString &path, GamsStandardDataPaths)
            if (!res.contains(path)) res << path;
        return res; }
    }
}

QString CommonPaths::gamsConnectSchemaDir()
{
#ifdef __APPLE__
    QString GamsConnectSchemaDir      = "../../../Resources/apifiles/Python/gams/gams_connect/agents/schema";
    QString GmsPythonConnectSchemaDir = "../../../Resources/GMSPython/lib/python3.8/site-packages/gams/connect/agents/schema";
#elif defined(__unix__)
     QString GamsConnectSchemaDir = "apifiles/Python/gams/gams_connect/agents/schema";
     QString GmsPythonConnectSchemaDir = "GMSPython/lib/python3.8/site-packages/gams/connect/agents/schema";
#else
    QString GamsConnectSchemaDir      = "apifiles/Python/gams/gams_connect/agents/schema";
    QString GmsPythonConnectSchemaDir = "GMSPython/Lib/site-packages/gams/connect/agents/schema";
#endif
    QString dirpath = QDir::cleanPath(systemDir()+QDir::separator()+GmsPythonConnectSchemaDir);
    if (QDir(dirpath).exists())
        return dirpath;

    return QDir::cleanPath(systemDir()+QDir::separator()+GamsConnectSchemaDir);
}

QString CommonPaths::defaultWorkingDir()
{
    QDir defWorkingDir(userDocumentsDir() + "/workspace");
    if(!defWorkingDir.exists())
        defWorkingDir.mkpath(".");
    return defWorkingDir.path();
}

QString CommonPaths::absolutFilePath(const QString &filePath)
{
    QFileInfo fi(filePath);
    return fi.absoluteFilePath();
}

QString CommonPaths::absolutPath(const QString &dir)
{
    if (dir.isEmpty())
        return "";
    QDir d(dir);
    return d.absolutePath();
}

QString CommonPaths::nativePathForProcess(const QString &filePath)
{
#if defined(__unix__) || defined(__APPLE__)
    return QDir::toNativeSeparators(filePath);
#else
    return "\""+QDir::toNativeSeparators(filePath)+"\"";
#endif
}

QString CommonPaths::configFile()
{
    QDir configFile(systemDir() + "/" + ConfigFile);
    return configFile.path();
}

QString CommonPaths::licenseFile()
{
    return LicenseFile;
}

QString CommonPaths::changelog()
{
#ifdef __APPLE__
    return QDir::cleanPath(MacOSCocoaBridge::bundlePath().append("/Contents/Resources/Changelog"));
#elif defined(__unix__)
    return QDir::cleanPath(QCoreApplication::applicationDirPath().append("/../resources/Changelog"));
#else
    return QDir::cleanPath(QCoreApplication::applicationDirPath().append("/resources/Changelog"));
#endif
}

}
}
