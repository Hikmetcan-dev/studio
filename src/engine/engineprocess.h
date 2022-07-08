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
#ifndef GAMS_STUDIO_ENGINE_ENGINEPROCESS_H
#define GAMS_STUDIO_ENGINE_ENGINEPROCESS_H

#include "process.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QFileInfo>

class QNetworkReply;
class QSslError;
class QDir;

namespace gams {
namespace studio {
namespace engine {

struct QuotaData;
class EngineManager;

/// \brief The EngineProcess controls all steps to run a job on GAMS Engine
/// This class works in four process steps:
/// 1. compile gms on the local machine
/// 2. monitor the remote job
/// 3. get result file
/// 4. unpack result file and finish
class EngineProcess final : public AbstractGamsProcess
{
    Q_OBJECT
public:
    EngineProcess(QObject *parent = nullptr);
    ~EngineProcess() override;

    static void startupInit();

    void execute() override;
    void interrupt() override;
    void terminate() override;
    void terminateLocal() override;
    TerminateOption terminateOption() override { return termRemote; }

    void setParameters(const QStringList &parameters) override;
    void forcePreviousWork();
    void setHasPreviousWorkOption(bool value);
    bool hasPreviousWorkOption() const { return mHasPreviousWorkOption; }
    QProcess::ProcessState state() const override;
    bool setUrl(const QString &url);
    QUrl url();
    void authorize(const QString &username, const QString &password, int expireMinutes);
    void initUsername(const QString &user);
    void setAuthToken(const QString &bearerToken);
    QString authToken() const { return mAuthToken; }
    void setNamespace(const QString &nSpace);
    void setIgnoreSslErrorsCurrentUrl(bool ignore);
    bool isIgnoreSslErrors() const;
    void listJobs();
    void listNamespaces();
    void sendPostLoginRequests();
    void setSelectedInstance(const QString &selectedInstance);
    void getVersions();
    void addLastCert();
    bool inKubernetes() const;

    bool forceGdx() const;
    void setForceGdx(bool forceGdx);
    void abortRequests();

signals:
    void authorized(const QString &token);
    void authorizeError(const QString &error);
    void procStateChanged(gams::studio::AbstractProcess *proc, gams::studio::ProcState progress);
    void requestAcceptSslErrors();
    void sslValidation(const QString &errorMessage);
    void reListJobs(qint32 count);
    void reListJobsError(const QString &error);
    void reListNamspaces(const QStringList &list);
    void reListNamespacesError(const QString &error);
    void reVersion(const QString &engineVersion, const QString &gamsVersion, bool isInKubernetes);
    void reVersionError(const QString &errorText);
    void reUserInstances(const QList<QPair<QString, QList<double> > > instances, const QString &defaultLabel);
    void reUserInstancesError(const QString &errorText);
    void quotaHint(const QStringList &diskHint, const QStringList &volumeHint);
    void sslSelfSigned(int sslError);
    void allPendingRequestsCompleted();
    void releaseGdxFile(const QString &gdxFilePath);
    void reloadGdxFile(const QString &gdxFilePath);

protected slots:
    void completed(int exitCode) override;
    void rePing(const QString &value);
    void reCreateJob(const QString &message, const QString &token);
    void reGetJobStatus(qint32 status, qint32 gamsExitCode);
    void reKillJob(const QString &text);
    void reGetLog(const QByteArray &data);
    void reQuota(const QList<gams::studio::engine::QuotaData *> data);
    void reQuotaError(const QString &errorText);
    void jobIsQueued();
    void reGetOutputFile(const QByteArray &data);
    void reError(const QString &errorText);
    void reAuthorize(const QString &token);

private slots:
    void pollStatus();
    void compileCompleted(int exitCode, QProcess::ExitStatus exitStatus);
    void packCompleted(int exitCode, QProcess::ExitStatus exitStatus);
    void pack2Completed(int exitCode, QProcess::ExitStatus exitStatus);
    void unpackCompleted(int exitCode, QProcess::ExitStatus exitStatus);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void parseUnZipStdOut(const QByteArray &data);
    void subProcStateChanged(QProcess::ProcessState newState);
    void reVersionIntern(const QString &engineVersion, const QString &gamsVersion, bool isInKubernetes);

private:
    void setProcState(ProcState newState);
    QStringList compileParameters() const;
    QStringList remoteParameters() const;
    QByteArray convertReferences(const QByteArray &data);
    void startPacking();
    void startPacking2();
    void startUnpacking();
    void handleResultFiles();
    void mkDirsAndMoveFiles(const QDir &srcDir, const QDir &destDir, bool inBase = false);
    void moveFiles(const QDir &srcDir, const QDir &destDir, bool inBase = false);
    QString modelName() const;
    bool addFilenames(const QString &efiFile, QStringList &list);

    EngineManager *mManager;
    QString mHost;
    QString mNamespace;
    QString mAuthToken;
    QString mWorkPath;
    QString mOutPath;
    QString mModelName;
    QString mEngineVersion;
    QString mGamsVersion;
    bool mInKubernetes = false;
    QString mUserInstance;
    bool mHasPreviousWorkOption = false;
    bool mForcePreviousWork = false;
    bool mForceGdx = true;
    QByteArray mRemoteWorkDir;
    bool mInParameterBlock = false;
    bool mStoredIgnoreSslState = false;
    QElapsedTimer mQueuedTimer;
    QList<QFileInfo> mProtectedFiles;

    QString mJobNumber;
    QString mJobPassword;
    ProcState mProcState;
    QTimer mPollTimer;
    AbstractGamsProcess *mSubProc = nullptr;

    enum JobStatusEnum {jsInvalid, jsDone, jsRunning, jsWaiting, jsUnknownJob, jsBadPassword};
    static const QHash<QString, JobStatusEnum> CJobStatus;
};

} // namespace engine
} // namespace studio
} // namespace gams

#endif // GAMS_STUDIO_ENGINE_ENGINEPROCESS_H
