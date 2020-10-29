/**
 * GAMS Engine
 * GAMS Engine let's you register, solve and get results of GAMS Models. It has namespace management system so you can restrict your users to certain set of models.
 *
 * The version of the OpenAPI document: latest
 *
 * NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

#ifndef OAI_OAIAuthApi_H
#define OAI_OAIAuthApi_H

#include "OAIHttpRequest.h"

#include "OAIMessage.h"
#include "OAIModel_auth_token.h"
#include <QString>

#include <QObject>
#include <QNetworkAccessManager>

namespace OpenAPI {

class OAIAuthApi : public QObject {
    Q_OBJECT

public:
    OAIAuthApi(const QString &scheme = "http", const QString &host = "localhost", int port = 0, const QString &basePath = "", const int timeOut = 0);
    ~OAIAuthApi();

    void setScheme(const QString &scheme);
    void setHost(const QString &host);
    void setPort(int port);
    void setBasePath(const QString &basePath);
    void setTimeOut(const int timeOut);
    void setWorkingDirectory(const QString &path);
    void setNetworkAccessManager(QNetworkAccessManager* manager);
    void addHeaders(const QString &key, const QString &value);
    void enableRequestCompression();
    void enableResponseCompression();
    void abortRequests();

    void createJWTToken();
    void postW(const QString &username, const QString &password);

private:
    QString _scheme, _host;
    int _port;
    QString _basePath;
    int _timeOut;
    QString _workingDirectory;
    QNetworkAccessManager* _manager;
    QMap<QString, QString> defaultHeaders;
    bool isResponseCompressionEnabled;
    bool isRequestCompressionEnabled;

    void createJWTTokenCallback(OAIHttpRequestWorker *worker);
    void postWCallback(OAIHttpRequestWorker *worker);

signals:

    void createJWTTokenSignal(OAIModel_auth_token summary);
    void postWSignal(OAIModel_auth_token summary);

    void createJWTTokenSignalFull(OAIHttpRequestWorker *worker, OAIModel_auth_token summary);
    void postWSignalFull(OAIHttpRequestWorker *worker, OAIModel_auth_token summary);

    void createJWTTokenSignalE(OAIModel_auth_token summary, QNetworkReply::NetworkError error_type, QString error_str);
    void postWSignalE(OAIModel_auth_token summary, QNetworkReply::NetworkError error_type, QString error_str);

    void createJWTTokenSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void postWSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);

    void abortRequestsSignal(); 
};

} // namespace OpenAPI
#endif
