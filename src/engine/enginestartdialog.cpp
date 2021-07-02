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
 */
#include "enginestartdialog.h"
#include "ui_enginestartdialog.h"
#include "logger.h"
#include "engineprocess.h"
#include "theme.h"
#include <QPushButton>
#include <QEvent>
#include <QUrl>
#include <QSslError>

namespace gams {
namespace studio {
namespace engine {

const QString EngineStartDialog::CUnavailable("-");

EngineStartDialog::EngineStartDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::EngineStartDialog), mProc(nullptr)
{
    ui->setupUi(this);
    setCanStart(false);
    QFont f = ui->laWarn->font();
    f.setBold(true);
    ui->laWarn->setFont(f);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &EngineStartDialog::buttonClicked);
    connect(ui->edUrl, &QLineEdit::textEdited, this, [this]() { mUrlChangedTimer.start(); });
    connect(ui->edUrl, &QLineEdit::textChanged, this, &EngineStartDialog::updateStates);
    connect(ui->edNamespace, &QLineEdit::textChanged, this, &EngineStartDialog::updateStates);
    connect(ui->edUser, &QLineEdit::textChanged, this, &EngineStartDialog::updateStates);
    connect(ui->edPassword, &QLineEdit::textChanged, this, &EngineStartDialog::updateStates);
    connect(ui->bAlways, &QPushButton::clicked, this, &EngineStartDialog::btAlwaysClicked);
    connect(ui->cbForceGdx, &QCheckBox::stateChanged, this, &EngineStartDialog::forceGdxStateChanged);
    connect(ui->cbAcceptCert, &QCheckBox::stateChanged, this, &EngineStartDialog::certAcceptChanged);

    if (Theme::instance()->baseTheme(Theme::instance()->activeTheme()) != 0)
        ui->laLogo->setPixmap(QPixmap(QString::fromUtf8(":/img/engine-logo-w")));
    GamsProcess gp;
    QString about = gp.aboutGAMS();
    QRegExp regex("^GAMS Release\\s*:\\s+(\\d\\d\\.\\d).*");
    if (regex.exactMatch(about))
        mLocalGamsVersion = regex.cap(regex.captureCount()).split('.');
    updateStates();
    ui->edUrl->installEventFilter(this);
    mConnectStateUpdater.setSingleShot(true);
    mConnectStateUpdater.setInterval(100);
    connect(&mConnectStateUpdater, &QTimer::timeout, this, &EngineStartDialog::updateConnectStateAppearance);
    mUrlChangedTimer.setSingleShot(true);
    mUrlChangedTimer.setInterval(200);
    connect(&mUrlChangedTimer, &QTimer::timeout, this, [this]() { urlEdited(ui->edUrl->text()); });
}

EngineStartDialog::~EngineStartDialog()
{
    if (mProc) mProc = nullptr;
    delete ui;
}

void EngineStartDialog::hiddenCheck()
{
    mHiddenCheck = true;
    getVersion();
}

void EngineStartDialog::setProcess(EngineProcess *process)
{
    mProc = process;
    connect(mProc, &EngineProcess::reVersion, this, &EngineStartDialog::reVersion);
    connect(mProc, &EngineProcess::reVersionError, this, &EngineStartDialog::reVersionError);
    connect(mProc, &EngineProcess::sslSelfSigned, this, &EngineStartDialog::selfSignedCertFound);
    mProc->setForceGdx(ui->cbForceGdx->isChecked());
    urlEdited(ui->edUrl->text());
}

EngineProcess *EngineStartDialog::process() const
{
    return mProc;
}

void EngineStartDialog::setAcceptCert()
{
    if (!ui->cbAcceptCert->isVisible())
        ui->cbAcceptCert->setVisible(true);
    ui->cbAcceptCert->setChecked(true);
}

bool EngineStartDialog::isCertAccepted()
{
    return ui->cbAcceptCert->isChecked();
}

void EngineStartDialog::initData(const QString &_url, const QString &_nSpace, const QString &_user, const QString &_pw, bool _forceGdx)
{
    ui->edUrl->setText(cleanUrl(_url));
    ui->edNamespace->setText(_nSpace);
    ui->edUser->setText(_user);
    ui->edPassword->setText(_pw);
    ui->cbForceGdx->setChecked(_forceGdx);
}

QString EngineStartDialog::url() const
{
    return mValidUrl;
}

QString EngineStartDialog::nSpace() const
{
    return ui->edNamespace->text();
}

QString EngineStartDialog::user() const
{
    return ui->edUser->text();
}

QString EngineStartDialog::password() const
{
    return ui->edPassword->text();
}

bool EngineStartDialog::forceGdx() const
{
    return ui->cbForceGdx->isChecked();
}

void EngineStartDialog::focusEmptyField()
{
    if (ui->edUrl->text().isEmpty()) ui->edUrl->setFocus();
    else if (ui->edNamespace->text().isEmpty()) ui->edNamespace->setFocus();
    else if (ui->edUser->text().isEmpty()) ui->edUser->setFocus();
    else if (ui->edPassword->text().isEmpty()) ui->edPassword->setFocus();
}

void EngineStartDialog::setEngineVersion(QString version)
{
    ui->laEngineVersion->setText(version);
}

bool EngineStartDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->edUrl && event->type() == QEvent::FocusOut)
        updateUrlEdit();
    return QDialog::eventFilter(watched, event);
}

void EngineStartDialog::prepareOpen()
{
    if (!ui->cbAcceptCert->isVisible()) ui->cbAcceptCert->setVisible(true);
}

void EngineStartDialog::updateUrlEdit()
{
    QString url = cleanUrl(mValidUrl.isEmpty() ? mValidSelfCertUrl : mValidUrl);
    UrlCheck prot = protocol(ui->edUrl->text().trimmed());
    if (!url.isEmpty() && (prot == ucNone || prot == protocol(url)))
        ui->edUrl->setText(url);
}

QDialogButtonBox::StandardButton EngineStartDialog::standardButton(QAbstractButton *button) const
{
    if (button == ui->bAlways)
        return QDialogButtonBox::YesToAll;
    return ui->buttonBox->standardButton(button);
}

void EngineStartDialog::closeEvent(QCloseEvent *event)
{
    if (mProc) mProc->abortRequests();
    disconnect(ui->cbAcceptCert, &QCheckBox::stateChanged, this, &EngineStartDialog::certAcceptChanged);
    ui->cbAcceptCert->setChecked(false);
    connect(ui->cbAcceptCert, &QCheckBox::stateChanged, this, &EngineStartDialog::certAcceptChanged);
    QDialog::closeEvent(event);
}

void EngineStartDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    setFixedSize(size());
    ui->cbAcceptCert->setVisible(!mProc || (mProc->isIgnoreSslErrors() && protocol(mRawUrl) != ucHttp));
}

void EngineStartDialog::buttonClicked(QAbstractButton *button)
{
    bool always = button == ui->bAlways;
    bool start = always || ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok;
    if (mForcePreviousWork && mProc) mProc->forcePreviousWork();
    if (!start) {
        disconnect(ui->cbAcceptCert, &QCheckBox::stateChanged, this, &EngineStartDialog::certAcceptChanged);
        ui->cbAcceptCert->setChecked(false);
        connect(ui->cbAcceptCert, &QCheckBox::stateChanged, this, &EngineStartDialog::certAcceptChanged);
    }
    emit ready(start, always);
}

void EngineStartDialog::getVersion()
{
    setConnectionState(scsWaiting);
    if (mProc) {
        if (mProc->setUrl(mUrl)) {
            mUrlChanged = false;
            mProc->getVersions();
            return;
        }
    }
    mConnectState = scsNone;
    updateConnectStateAppearance();
}

void EngineStartDialog::setCanStart(bool canStart)
{
    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (!bOk) return;
    canStart = canStart && !ui->edUrl->text().isEmpty() && !ui->edNamespace->text().isEmpty()
            && !ui->edUser->text().isEmpty() && !ui->edPassword->text().isEmpty()
            && (!ui->cbAcceptCert->isVisible() || ui->cbAcceptCert->isChecked());
    if (canStart != bOk->isEnabled())
        bOk->setEnabled(canStart);
    if (canStart != ui->bAlways->isEnabled())
        ui->bAlways->setEnabled(canStart);
}

void EngineStartDialog::setConnectionState(ServerConnectionState state)
{
//    DEB() << "ConnectionState: " << state << "     isIgnoreSsl:" << (mProc ? mProc->isIgnoreSslErrors() : -1);
    mConnectState = state;
    mConnectStateUpdater.start();
}

void EngineStartDialog::certAcceptChanged()
{
    mProc->abortRequests();
    mProc->setIgnoreSslErrorsCurrentUrl(ui->cbAcceptCert->isChecked());
    urlEdited(ui->edUrl->text());
}

void EngineStartDialog::hideCert()
{
    ui->cbAcceptCert->setVisible(false);
}

void EngineStartDialog::urlEdited(const QString &text)
{
//    DEB() << "----------------------- " << text;
    mProc->abortRequests();
    initUrlAndChecks(text);
    getVersion();
}

void EngineStartDialog::updateStates()
{
    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (!bOk) return;
    bool enabled = !ui->edUrl->text().isEmpty() && !ui->edNamespace->text().isEmpty()
            && !ui->edUser->text().isEmpty() && !ui->edPassword->text().isEmpty();
    if (enabled && ui->laEngineVersion->text() == CUnavailable) {
        getVersion();
    }
    setConnectionState(mConnectState);
}

void EngineStartDialog::btAlwaysClicked()
{
    buttonClicked(ui->bAlways);
}

void EngineStartDialog::reVersion(const QString &engineVersion, const QString &gamsVersion)
{
    mUrlChecks = ucNone;
    mEngineVersion = engineVersion;
    mGamsVersion = gamsVersion;
    UrlCheck protUser = protocol(cleanUrl(ui->edUrl->text()));
    UrlCheck protServer = protocol(mProc->url().toString());
    if (protUser != ucNone && protUser != protServer) {
        setConnectionState(protServer == ucApiHttp ? scsHttpFound : scsHttpsFound);
    } else {
        mValidUrl = mProc->url().toString();
        setConnectionState(scsValid);
        if (focusWidget() != ui->edUrl)
            updateUrlEdit();
    }
}

void EngineStartDialog::reVersionError(const QString &errorText)
{
    if (!mValidUrl.isEmpty()) return;
    Q_UNUSED(errorText)

    if (mUrlChanged) {
        getVersion();
        return;
    }
    // if the raw input failed, try next protocol/api combination
    if (!mLastSslError && fetchNextUrl()) {
//        DEB() << "          ------next: " << mUrl;
        getVersion();
        return;
    }
    // neither user-input nor user-input with modifications is valid, so reset mUrl to user-input
    setConnectionState(scsInvalid);
    if (mUrl != mValidUrl)
        mUrl = ui->edUrl->text();

    // if server not found on hidden dialog - open dialog anyway
    if (!isVisible() && mHiddenCheck) {
        open();
    }
}

void EngineStartDialog::selfSignedCertFound(int sslError)
{
    mValidSelfCertUrl = mProc->url().toString();
    setConnectionState(scsHttpsSelfSignedFound);
    mLastSslError = sslError;
    if (mInitialProtocol != ucHttp)
        ui->cbAcceptCert->setVisible(true);
}

void EngineStartDialog::forceGdxStateChanged(int state)
{
    if (mProc) mProc->setForceGdx(state != 0);
}

void EngineStartDialog::updateConnectStateAppearance()
{
    switch (mConnectState) {
    case scsNone: {
        ui->laEngGamsVersion->setText("");
        ui->laEngineVersion->setText(CUnavailable);
        ui->laWarn->setText("No GAMS Engine server");
        ui->laWarn->setToolTip("");
        mForcePreviousWork = false;
        setCanStart(false);
    } break;
    case scsWaiting: {
        ui->laEngGamsVersion->setText("");
        ui->laEngineVersion->setText(CUnavailable);
        ui->laWarn->setText("Waiting for server ...");
        ui->laWarn->setToolTip("");
        mForcePreviousWork = false;
        setCanStart(false);
    } break;
    case scsHttpFound: {
        ui->laWarn->setText("HTTP found.");
        ui->laWarn->setToolTip("");
    } break;
    case scsHttpsFound: {
        ui->laWarn->setText("HTTPS found.");
        ui->laWarn->setToolTip("");
    } break;
    case scsHttpsSelfSignedFound: {
        ui->laWarn->setText("Self-signed HTTPS found.");
        ui->laWarn->setToolTip("");
    } break;
    case scsValid: {
        ui->laEngineVersion->setText("Engine "+mEngineVersion);
        ui->laEngGamsVersion->setText("GAMS "+mGamsVersion);
        if (!mProc->hasPreviousWorkOption()) {
            bool newerGamsVersion = false;
            QStringList engineGamsVersion = QString(mGamsVersion).split('.');
            if (mLocalGamsVersion.at(0).toInt() > engineGamsVersion.at(0).toInt())
                newerGamsVersion = true;
            if (mLocalGamsVersion.at(0).toInt() == engineGamsVersion.at(0).toInt() &&
                mLocalGamsVersion.at(1).toInt() > engineGamsVersion.at(1).toInt())
                newerGamsVersion = true;
            if (newerGamsVersion) {
                ui->laWarn->setText("Newer local GAMS: Added \"previousWork=1\"");
                ui->laWarn->setToolTip("Set \"previousWork=0\" to suppress this");
                mForcePreviousWork = true;
            } else {
                ui->laWarn->setText("");
                ui->laWarn->setToolTip("");
                mForcePreviousWork = false;
            }
            if (!isVisible() && mHiddenCheck) {
                // hidden start
                if (mForcePreviousWork && mProc) mProc->forcePreviousWork();
                emit ready(true, true);
            }
        } else {
            ui->laWarn->setText("");
            ui->laWarn->setToolTip("");
            mForcePreviousWork = false;
        }
        setCanStart(true);
    } break;
    case scsInvalid: {
        if (!mValidSelfCertUrl.isEmpty()) {
            ui->laEngGamsVersion->setText("");
            ui->laEngineVersion->setText(CUnavailable);
            if (mLastSslError==int(QSslError::CertificateStatusUnknown))
                ui->laWarn->setText(mInitialProtocol == ucHttp ? "HTTPS found with certification error"
                                                               : "Certification error encountered");
            else
                ui->laWarn->setText(mInitialProtocol == ucHttp ? "HTTPS found with self-signed certificate"
                                                               : "Self-signed certificate found");
            if (mInitialProtocol == ucHttp)
                ui->laWarn->setToolTip("Change the URL to " + QString(mInitialProtocol == ucHttp ? "HTTPS" : "HTTP"));
            else
                ui->laWarn->setToolTip("Use checkbox below to connect anyway");
        } else {
            ui->laEngGamsVersion->setText("");
            ui->laEngineVersion->setText(CUnavailable);
            ui->laWarn->setText("No GAMS Engine server");
            ui->laWarn->setToolTip("");
        }
        mForcePreviousWork = false;
        setCanStart(false);
    } break;
    }
}

void EngineStartDialog::initUrlAndChecks(QString url)
{
    mValidSelfCertUrl = "";
    mLastSslError = 0;
    mUrlChanged = true;
    mValidUrl = QString();
    mUrl = url.trimmed();
    mUrlChecks = ucAll;
    mInitialProtocol = protocol(mUrl);
    if (!mUrl.endsWith('/'))
            mUrl += '/';
    if (mUrl.endsWith("/api/", Qt::CaseInsensitive)) {
        mUrlChecks.setFlag(ucApiHttps, false);
        mUrlChecks.setFlag(ucApiHttp, false);
    }
    if (mInitialProtocol == ucHttp) {
        mUrlChecks.setFlag(ucHttp, false);
    } else {
        if (mInitialProtocol == ucNone)
            mUrl = "https://" + mUrl;
        mUrlChecks.setFlag(ucHttps, false);
        mInitialProtocol = ucHttps;
    }
    mUrl = cleanUrl(mUrl);
    mRawUrl = mUrl;
    ui->cbAcceptCert->setVisible(mProc->isIgnoreSslErrors() && protocol(mRawUrl) != ucHttp);
}

bool EngineStartDialog::fetchNextUrl()
{
    mLastSslError = 0;
    // first check for a missing "api/"
    if (!mUrlChecks.testFlag(ucHttps) && mUrlChecks.testFlag(ucApiHttps)) {
        mUrl = "https" + mRawUrl.mid(mRawUrl.indexOf("://"), mRawUrl.length()) + "api/";
        if (mUrl.contains(":443/"))
            mUrl.replace(":443/", "/");
        mUrlChecks.setFlag(ucApiHttps, false);
        return true;
    }
    if (!mUrlChecks.testFlag(ucHttp) && mUrlChecks.testFlag(ucApiHttp)) {
        mUrl = "http" + mRawUrl.mid(mRawUrl.indexOf("://"), mRawUrl.length()) + "api/";
        mUrlChecks.setFlag(ucApiHttp, false);
        return true;
    }
    // then check for the protocol
    if (mUrlChecks.testFlag(ucHttps)) {
        mUrl = cleanUrl("https" + mRawUrl.mid(mRawUrl.indexOf("://"), mRawUrl.length()));
        mUrlChecks.setFlag(ucHttps, false);
        return true;
    }
    if (mUrlChecks.testFlag(ucHttp)) {
        mUrl = cleanUrl("http" + mRawUrl.mid(mRawUrl.indexOf("://"), mRawUrl.length()));
        mUrlChecks.setFlag(ucHttp, false);
        return true;
    }
    return false;
}

EngineStartDialog::UrlCheck EngineStartDialog::protocol(QString url)
{
    if (url.startsWith("http://", Qt::CaseInsensitive))
        return ucHttp;
    if (url.startsWith("https://", Qt::CaseInsensitive))
        return ucHttps;
    return ucNone;
}

QString EngineStartDialog::cleanUrl(const QString url)
{
    QString res = url.trimmed();
    if (res.startsWith("http://", Qt::CaseInsensitive)) {
        if (res.contains(":80/"))
            res.replace(":80/", "/");
    } else if (res.startsWith("https://", Qt::CaseInsensitive)) {
        if (res.contains(":443/"))
            res.replace(":443/", "/");
    }
    return res;
}

} // namespace engine
} // namespace studio
} // namespace gams

