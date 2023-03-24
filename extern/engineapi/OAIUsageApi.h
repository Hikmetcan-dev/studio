/**
 * GAMS Engine
 * With GAMS Engine you can register and solve GAMS models. It has a namespace management system, so you can restrict your users to certain models.
 *
 * The version of the OpenAPI document: 23.02.18
 *
 * NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

#ifndef OAI_OAIUsageApi_H
#define OAI_OAIUsageApi_H

#include "OAIHelpers.h"
#include "OAIHttpRequest.h"
#include "OAIServerConfiguration.h"
#include "OAIOauth.h"

#include "OAIMessage.h"
#include "OAIModel_instance_info_full.h"
#include "OAIModel_usage.h"
#include "OAIModel_userinstance_info.h"
#include "OAIQuota.h"
#include <QString>

#include <QObject>
#include <QByteArray>
#include <QStringList>
#include <QList>
#include <QNetworkAccessManager>

namespace OpenAPI {

class OAIUsageApi : public QObject {
    Q_OBJECT

public:
    OAIUsageApi(const int timeOut = 0);
    ~OAIUsageApi();

    void initializeServerConfigs();
    int setDefaultServerValue(int serverIndex,const QString &operation, const QString &variable,const QString &val);
    void setServerIndex(const QString &operation, int serverIndex);
    void setApiKey(const QString &apiKeyName, const QString &apiKey);
    void setBearerToken(const QString &token);
    void setUsername(const QString &username);
    void setPassword(const QString &password);
    void setTimeOut(const int timeOut);
    void setWorkingDirectory(const QString &path);
    void setNetworkAccessManager(QNetworkAccessManager* manager);
    int addServerConfiguration(const QString &operation, const QUrl &url, const QString &description = "", const QMap<QString, OAIServerVariable> &variables = QMap<QString, OAIServerVariable>());
    void setNewServerForAllOperations(const QUrl &url, const QString &description = "", const QMap<QString, OAIServerVariable> &variables =  QMap<QString, OAIServerVariable>());
    void setNewServer(const QString &operation, const QUrl &url, const QString &description = "", const QMap<QString, OAIServerVariable> &variables =  QMap<QString, OAIServerVariable>());
    void addHeaders(const QString &key, const QString &value);
    void enableRequestCompression();
    void enableResponseCompression();
    void abortRequests();
    QString getParamStylePrefix(const QString &style);
    QString getParamStyleSuffix(const QString &style);
    QString getParamStyleDelimiter(const QString &style, const QString &name, bool isExplode);

    /**
    * @param[in]  label QString [required]
    * @param[in]  cpu_request double [required]
    * @param[in]  memory_request qint32 [required]
    * @param[in]  multiplier double [required]
    * @param[in]  workspace_request qint32 [required]
    * @param[in]  tolerations QList<QString> [optional]
    * @param[in]  node_selectors QList<QString> [optional]
    * @param[in]  gams_license QString [optional]
    */
    void createInstance(const QString &label, const double &cpu_request, const qint32 &memory_request, const double &multiplier, const qint32 &workspace_request, const ::OpenAPI::OptionalParam<QList<QString>> &tolerations = ::OpenAPI::OptionalParam<QList<QString>>(), const ::OpenAPI::OptionalParam<QList<QString>> &node_selectors = ::OpenAPI::OptionalParam<QList<QString>>(), const ::OpenAPI::OptionalParam<QString> &gams_license = ::OpenAPI::OptionalParam<QString>());

    /**
    * @param[in]  label QString [required]
    */
    void deleteInstance(const QString &label);

    /**
    * @param[in]  username QString [required]
    * @param[in]  field QString [optional]
    */
    void deleteQuota(const QString &username, const ::OpenAPI::OptionalParam<QString> &field = ::OpenAPI::OptionalParam<QString>());

    /**
    * @param[in]  username QString [required]
    */
    void deleteUserInstances(const QString &username);


    void getInstances();

    /**
    * @param[in]  username QString [required]
    */
    void getQuota(const QString &username);

    /**
    * @param[in]  username QString [required]
    * @param[in]  recursive bool [optional]
    * @param[in]  from_datetime QDateTime [optional]
    * @param[in]  to_datetime QDateTime [optional]
    * @param[in]  x_fields QString [optional]
    * @param[in]  token QList<QString> [optional]
    * @param[in]  hypercube_token QList<QString> [optional]
    */
    void getUsage(const QString &username, const ::OpenAPI::OptionalParam<bool> &recursive = ::OpenAPI::OptionalParam<bool>(), const ::OpenAPI::OptionalParam<QDateTime> &from_datetime = ::OpenAPI::OptionalParam<QDateTime>(), const ::OpenAPI::OptionalParam<QDateTime> &to_datetime = ::OpenAPI::OptionalParam<QDateTime>(), const ::OpenAPI::OptionalParam<QString> &x_fields = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QList<QString>> &token = ::OpenAPI::OptionalParam<QList<QString>>(), const ::OpenAPI::OptionalParam<QList<QString>> &hypercube_token = ::OpenAPI::OptionalParam<QList<QString>>());

    /**
    * @param[in]  username QString [required]
    */
    void getUserInstances(const QString &username);

    /**
    * @param[in]  label QString [required]
    * @param[in]  cpu_request double [required]
    * @param[in]  memory_request qint32 [required]
    * @param[in]  multiplier double [required]
    * @param[in]  workspace_request qint32 [required]
    * @param[in]  old_label QString [required]
    * @param[in]  tolerations QList<QString> [optional]
    * @param[in]  node_selectors QList<QString> [optional]
    * @param[in]  gams_license QString [optional]
    */
    void updateInstance(const QString &label, const double &cpu_request, const qint32 &memory_request, const double &multiplier, const qint32 &workspace_request, const QString &old_label, const ::OpenAPI::OptionalParam<QList<QString>> &tolerations = ::OpenAPI::OptionalParam<QList<QString>>(), const ::OpenAPI::OptionalParam<QList<QString>> &node_selectors = ::OpenAPI::OptionalParam<QList<QString>>(), const ::OpenAPI::OptionalParam<QString> &gams_license = ::OpenAPI::OptionalParam<QString>());

    /**
    * @param[in]  username QString [required]
    * @param[in]  parallel_quota double [optional]
    * @param[in]  volume_quota double [optional]
    * @param[in]  disk_quota qint32 [optional]
    */
    void updateQuota(const QString &username, const ::OpenAPI::OptionalParam<double> &parallel_quota = ::OpenAPI::OptionalParam<double>(), const ::OpenAPI::OptionalParam<double> &volume_quota = ::OpenAPI::OptionalParam<double>(), const ::OpenAPI::OptionalParam<qint32> &disk_quota = ::OpenAPI::OptionalParam<qint32>());

    /**
    * @param[in]  username QString [required]
    * @param[in]  default_label QString [required]
    */
    void updateUserDefaultInstance(const QString &username, const QString &default_label);

    /**
    * @param[in]  username QString [required]
    * @param[in]  default_label QString [required]
    * @param[in]  labels QList<QString> [optional]
    */
    void updateUserInstances(const QString &username, const QString &default_label, const ::OpenAPI::OptionalParam<QList<QString>> &labels = ::OpenAPI::OptionalParam<QList<QString>>());


private:
    QMap<QString,int> _serverIndices;
    QMap<QString,QList<OAIServerConfiguration>> _serverConfigs;
    QMap<QString, QString> _apiKeys;
    QString _bearerToken;
    QString _username;
    QString _password;
    int _timeOut;
    QString _workingDirectory;
    QNetworkAccessManager* _manager;
    QMap<QString, QString> _defaultHeaders;
    bool _isResponseCompressionEnabled;
    bool _isRequestCompressionEnabled;
    OAIHttpRequestInput _latestInput;
    OAIHttpRequestWorker *_latestWorker;
    QStringList _latestScope;
    OauthCode _authFlow;
    OauthImplicit _implicitFlow;
    OauthCredentials _credentialFlow;
    OauthPassword _passwordFlow;
    int _OauthMethod = 0;

    void createInstanceCallback(OAIHttpRequestWorker *worker);
    void deleteInstanceCallback(OAIHttpRequestWorker *worker);
    void deleteQuotaCallback(OAIHttpRequestWorker *worker);
    void deleteUserInstancesCallback(OAIHttpRequestWorker *worker);
    void getInstancesCallback(OAIHttpRequestWorker *worker);
    void getQuotaCallback(OAIHttpRequestWorker *worker);
    void getUsageCallback(OAIHttpRequestWorker *worker);
    void getUserInstancesCallback(OAIHttpRequestWorker *worker);
    void updateInstanceCallback(OAIHttpRequestWorker *worker);
    void updateQuotaCallback(OAIHttpRequestWorker *worker);
    void updateUserDefaultInstanceCallback(OAIHttpRequestWorker *worker);
    void updateUserInstancesCallback(OAIHttpRequestWorker *worker);

signals:

    void createInstanceSignal(OAIMessage summary);
    void deleteInstanceSignal(OAIMessage summary);
    void deleteQuotaSignal(OAIMessage summary);
    void deleteUserInstancesSignal(OAIMessage summary);
    void getInstancesSignal(QList<OAIModel_instance_info_full> summary);
    void getQuotaSignal(QList<OAIQuota> summary);
    void getUsageSignal(OAIModel_usage summary);
    void getUserInstancesSignal(OAIModel_userinstance_info summary);
    void updateInstanceSignal(OAIMessage summary);
    void updateQuotaSignal(OAIMessage summary);
    void updateUserDefaultInstanceSignal(OAIMessage summary);
    void updateUserInstancesSignal(OAIMessage summary);

    void createInstanceSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void deleteInstanceSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void deleteQuotaSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void deleteUserInstancesSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void getInstancesSignalFull(OAIHttpRequestWorker *worker, QList<OAIModel_instance_info_full> summary);
    void getQuotaSignalFull(OAIHttpRequestWorker *worker, QList<OAIQuota> summary);
    void getUsageSignalFull(OAIHttpRequestWorker *worker, OAIModel_usage summary);
    void getUserInstancesSignalFull(OAIHttpRequestWorker *worker, OAIModel_userinstance_info summary);
    void updateInstanceSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void updateQuotaSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void updateUserDefaultInstanceSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void updateUserInstancesSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);

    void createInstanceSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void deleteInstanceSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void deleteQuotaSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void deleteUserInstancesSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getInstancesSignalE(QList<OAIModel_instance_info_full> summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getQuotaSignalE(QList<OAIQuota> summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getUsageSignalE(OAIModel_usage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void getUserInstancesSignalE(OAIModel_userinstance_info summary, QNetworkReply::NetworkError error_type, QString error_str);
    void updateInstanceSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void updateQuotaSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void updateUserDefaultInstanceSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void updateUserInstancesSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);

    void createInstanceSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void deleteInstanceSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void deleteQuotaSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void deleteUserInstancesSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getInstancesSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getQuotaSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getUsageSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void getUserInstancesSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void updateInstanceSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void updateQuotaSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void updateUserDefaultInstanceSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void updateUserInstancesSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);

    void abortRequestsSignal();
    void allPendingRequestsCompleted();

public slots:
    void tokenAvailable();
    
};

} // namespace OpenAPI
#endif