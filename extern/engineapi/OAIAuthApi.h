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

#ifndef OAI_OAIAuthApi_H
#define OAI_OAIAuthApi_H

#include "OAIHelpers.h"
#include "OAIHttpRequest.h"
#include "OAIServerConfiguration.h"
#include "OAIOauth.h"

#include "OAIBad_input.h"
#include "OAIForwarded_token_response.h"
#include "OAIIdentity_provider.h"
#include "OAIIdentity_provider_ldap.h"
#include "OAIIdentity_provider_oauth2_with_secret.h"
#include "OAIMessage.h"
#include "OAIModel_auth_token.h"
#include "OAINot_found.h"
#include <QString>

#include <QObject>
#include <QByteArray>
#include <QStringList>
#include <QList>
#include <QNetworkAccessManager>

namespace OpenAPI {

class OAIAuthApi : public QObject {
    Q_OBJECT

public:
    OAIAuthApi(const int timeOut = 0);
    ~OAIAuthApi();

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
    * @param[in]  expires_in qint32 [optional]
    * @param[in]  scope QString [optional]
    */
    void createJWTToken(const ::OpenAPI::OptionalParam<qint32> &expires_in = ::OpenAPI::OptionalParam<qint32>(), const ::OpenAPI::OptionalParam<QString> &scope = ::OpenAPI::OptionalParam<QString>());

    /**
    * @param[in]  username QString [required]
    * @param[in]  password QString [required]
    * @param[in]  grant_type QString [optional]
    * @param[in]  scope QString [optional]
    * @param[in]  expires_in qint32 [optional]
    * @param[in]  access_scopes QList<QString> [optional]
    */
    void createJWTTokenJSON(const QString &username, const QString &password, const ::OpenAPI::OptionalParam<QString> &grant_type = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &scope = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<qint32> &expires_in = ::OpenAPI::OptionalParam<qint32>(), const ::OpenAPI::OptionalParam<QList<QString>> &access_scopes = ::OpenAPI::OptionalParam<QList<QString>>());

    /**
    * @param[in]  name QString [required]
    * @param[in]  label QString [required]
    * @param[in]  hidden bool [required]
    * @param[in]  host QString [required]
    * @param[in]  port qint32 [required]
    * @param[in]  uid QString [required]
    * @param[in]  encryption QString [required]
    * @param[in]  base QString [required]
    * @param[in]  user_filter QString [required]
    * @param[in]  bind_dn QString [optional]
    * @param[in]  password QString [optional]
    * @param[in]  verify_certificates bool [optional]
    * @param[in]  active_directory bool [optional]
    */
    void createLDAPIdentityProvider(const QString &name, const QString &label, const bool &hidden, const QString &host, const qint32 &port, const QString &uid, const QString &encryption, const QString &base, const QString &user_filter, const ::OpenAPI::OptionalParam<QString> &bind_dn = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &password = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<bool> &verify_certificates = ::OpenAPI::OptionalParam<bool>(), const ::OpenAPI::OptionalParam<bool> &active_directory = ::OpenAPI::OptionalParam<bool>());

    /**
    * @param[in]  name QString [required]
    * @param[in]  label QString [required]
    * @param[in]  hidden bool [required]
    * @param[in]  web_ui_client_id QString [required]
    * @param[in]  issuer QString [required]
    * @param[in]  web_ui_client_secret QString [optional]
    * @param[in]  device_client_id QString [optional]
    * @param[in]  override_audience QString [optional]
    * @param[in]  use_oidc_discover bool [optional]
    * @param[in]  use_oauth2_auth_server_metadata bool [optional]
    * @param[in]  authorization_endpoint QString [optional]
    * @param[in]  token_endpoint QString [optional]
    * @param[in]  jwks_uri QString [optional]
    * @param[in]  response_types_supported QList<QString> [optional]
    * @param[in]  grant_types_supported QList<QString> [optional]
    * @param[in]  end_session_endpoint QString [optional]
    * @param[in]  device_authorization_endpoint QString [optional]
    * @param[in]  request_scope_readonly QString [optional]
    * @param[in]  request_scope_namespaces QString [optional]
    * @param[in]  request_scope_jobs QString [optional]
    * @param[in]  request_scope_users QString [optional]
    * @param[in]  request_scope_hypercube QString [optional]
    * @param[in]  request_scope_cleanup QString [optional]
    * @param[in]  request_scope_licenses QString [optional]
    * @param[in]  request_scope_usage QString [optional]
    * @param[in]  request_scope_auth QString [optional]
    * @param[in]  request_scope_configuration QString [optional]
    */
    void createOAuth2IdentityProvider(const QString &name, const QString &label, const bool &hidden, const QString &web_ui_client_id, const QString &issuer, const ::OpenAPI::OptionalParam<QString> &web_ui_client_secret = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &device_client_id = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &override_audience = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<bool> &use_oidc_discover = ::OpenAPI::OptionalParam<bool>(), const ::OpenAPI::OptionalParam<bool> &use_oauth2_auth_server_metadata = ::OpenAPI::OptionalParam<bool>(), const ::OpenAPI::OptionalParam<QString> &authorization_endpoint = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &token_endpoint = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &jwks_uri = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QList<QString>> &response_types_supported = ::OpenAPI::OptionalParam<QList<QString>>(), const ::OpenAPI::OptionalParam<QList<QString>> &grant_types_supported = ::OpenAPI::OptionalParam<QList<QString>>(), const ::OpenAPI::OptionalParam<QString> &end_session_endpoint = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &device_authorization_endpoint = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_readonly = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_namespaces = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_jobs = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_users = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_hypercube = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_cleanup = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_licenses = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_usage = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_auth = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_configuration = ::OpenAPI::OptionalParam<QString>());

    /**
    * @param[in]  name QString [required]
    */
    void deleteIdentityProvider(const QString &name);

    /**
    * @param[in]  identity_provider_name QString [required]
    * @param[in]  code QString [required]
    * @param[in]  code_verifier QString [required]
    * @param[in]  redirect_uri QString [required]
    */
    void fetchOAuth2TokenOnBehalf(const QString &identity_provider_name, const QString &code, const QString &code_verifier, const QString &redirect_uri);


    void invalidateJWTTokens();


    void listAllIdentityProviders();

    /**
    * @param[in]  name QString [optional]
    */
    void listIdentityProviders(const ::OpenAPI::OptionalParam<QString> &name = ::OpenAPI::OptionalParam<QString>());

    /**
    * @param[in]  name QString [optional]
    * @param[in]  x_fields QString [optional]
    */
    void listLDAPIdentityProviders(const ::OpenAPI::OptionalParam<QString> &name = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &x_fields = ::OpenAPI::OptionalParam<QString>());

    /**
    * @param[in]  name QString [optional]
    * @param[in]  x_fields QString [optional]
    */
    void listOAuth2IdentityProviders(const ::OpenAPI::OptionalParam<QString> &name = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &x_fields = ::OpenAPI::OptionalParam<QString>());

    /**
    * @param[in]  provider_name QString [required]
    * @param[in]  username QString [required]
    * @param[in]  password QString [required]
    * @param[in]  grant_type QString [optional]
    * @param[in]  scope QString [optional]
    * @param[in]  expires_in qint32 [optional]
    * @param[in]  access_scopes QList<QString> [optional]
    */
    void loginWithLDAP(const QString &provider_name, const QString &username, const QString &password, const ::OpenAPI::OptionalParam<QString> &grant_type = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &scope = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<qint32> &expires_in = ::OpenAPI::OptionalParam<qint32>(), const ::OpenAPI::OptionalParam<QList<QString>> &access_scopes = ::OpenAPI::OptionalParam<QList<QString>>());

    /**
    * @param[in]  name QString [required]
    * @param[in]  label QString [required]
    * @param[in]  hidden bool [required]
    * @param[in]  host QString [required]
    * @param[in]  port qint32 [required]
    * @param[in]  uid QString [required]
    * @param[in]  encryption QString [required]
    * @param[in]  base QString [required]
    * @param[in]  user_filter QString [required]
    * @param[in]  bind_dn QString [optional]
    * @param[in]  password QString [optional]
    * @param[in]  verify_certificates bool [optional]
    * @param[in]  active_directory bool [optional]
    */
    void updateLDAPIdentityProvider(const QString &name, const QString &label, const bool &hidden, const QString &host, const qint32 &port, const QString &uid, const QString &encryption, const QString &base, const QString &user_filter, const ::OpenAPI::OptionalParam<QString> &bind_dn = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &password = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<bool> &verify_certificates = ::OpenAPI::OptionalParam<bool>(), const ::OpenAPI::OptionalParam<bool> &active_directory = ::OpenAPI::OptionalParam<bool>());

    /**
    * @param[in]  name QString [required]
    * @param[in]  label QString [required]
    * @param[in]  hidden bool [required]
    * @param[in]  web_ui_client_id QString [required]
    * @param[in]  issuer QString [required]
    * @param[in]  web_ui_client_secret QString [optional]
    * @param[in]  device_client_id QString [optional]
    * @param[in]  override_audience QString [optional]
    * @param[in]  use_oidc_discover bool [optional]
    * @param[in]  use_oauth2_auth_server_metadata bool [optional]
    * @param[in]  authorization_endpoint QString [optional]
    * @param[in]  token_endpoint QString [optional]
    * @param[in]  jwks_uri QString [optional]
    * @param[in]  response_types_supported QList<QString> [optional]
    * @param[in]  grant_types_supported QList<QString> [optional]
    * @param[in]  end_session_endpoint QString [optional]
    * @param[in]  device_authorization_endpoint QString [optional]
    * @param[in]  request_scope_readonly QString [optional]
    * @param[in]  request_scope_namespaces QString [optional]
    * @param[in]  request_scope_jobs QString [optional]
    * @param[in]  request_scope_users QString [optional]
    * @param[in]  request_scope_hypercube QString [optional]
    * @param[in]  request_scope_cleanup QString [optional]
    * @param[in]  request_scope_licenses QString [optional]
    * @param[in]  request_scope_usage QString [optional]
    * @param[in]  request_scope_auth QString [optional]
    * @param[in]  request_scope_configuration QString [optional]
    */
    void updateOAuth2IdentityProvider(const QString &name, const QString &label, const bool &hidden, const QString &web_ui_client_id, const QString &issuer, const ::OpenAPI::OptionalParam<QString> &web_ui_client_secret = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &device_client_id = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &override_audience = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<bool> &use_oidc_discover = ::OpenAPI::OptionalParam<bool>(), const ::OpenAPI::OptionalParam<bool> &use_oauth2_auth_server_metadata = ::OpenAPI::OptionalParam<bool>(), const ::OpenAPI::OptionalParam<QString> &authorization_endpoint = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &token_endpoint = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &jwks_uri = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QList<QString>> &response_types_supported = ::OpenAPI::OptionalParam<QList<QString>>(), const ::OpenAPI::OptionalParam<QList<QString>> &grant_types_supported = ::OpenAPI::OptionalParam<QList<QString>>(), const ::OpenAPI::OptionalParam<QString> &end_session_endpoint = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &device_authorization_endpoint = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_readonly = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_namespaces = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_jobs = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_users = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_hypercube = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_cleanup = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_licenses = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_usage = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_auth = ::OpenAPI::OptionalParam<QString>(), const ::OpenAPI::OptionalParam<QString> &request_scope_configuration = ::OpenAPI::OptionalParam<QString>());


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

    void createJWTTokenCallback(OAIHttpRequestWorker *worker);
    void createJWTTokenJSONCallback(OAIHttpRequestWorker *worker);
    void createLDAPIdentityProviderCallback(OAIHttpRequestWorker *worker);
    void createOAuth2IdentityProviderCallback(OAIHttpRequestWorker *worker);
    void deleteIdentityProviderCallback(OAIHttpRequestWorker *worker);
    void fetchOAuth2TokenOnBehalfCallback(OAIHttpRequestWorker *worker);
    void invalidateJWTTokensCallback(OAIHttpRequestWorker *worker);
    void listAllIdentityProvidersCallback(OAIHttpRequestWorker *worker);
    void listIdentityProvidersCallback(OAIHttpRequestWorker *worker);
    void listLDAPIdentityProvidersCallback(OAIHttpRequestWorker *worker);
    void listOAuth2IdentityProvidersCallback(OAIHttpRequestWorker *worker);
    void loginWithLDAPCallback(OAIHttpRequestWorker *worker);
    void updateLDAPIdentityProviderCallback(OAIHttpRequestWorker *worker);
    void updateOAuth2IdentityProviderCallback(OAIHttpRequestWorker *worker);

signals:

    void createJWTTokenSignal(OAIModel_auth_token summary);
    void createJWTTokenJSONSignal(OAIModel_auth_token summary);
    void createLDAPIdentityProviderSignal(OAIMessage summary);
    void createOAuth2IdentityProviderSignal(OAIMessage summary);
    void deleteIdentityProviderSignal(OAIMessage summary);
    void fetchOAuth2TokenOnBehalfSignal(OAIForwarded_token_response summary);
    void invalidateJWTTokensSignal(OAIMessage summary);
    void listAllIdentityProvidersSignal(QList<OAIIdentity_provider> summary);
    void listIdentityProvidersSignal(QList<OAIIdentity_provider> summary);
    void listLDAPIdentityProvidersSignal(OAIIdentity_provider_ldap summary);
    void listOAuth2IdentityProvidersSignal(QList<OAIIdentity_provider_oauth2_with_secret> summary);
    void loginWithLDAPSignal(OAIModel_auth_token summary);
    void updateLDAPIdentityProviderSignal(OAIMessage summary);
    void updateOAuth2IdentityProviderSignal(OAIMessage summary);

    void createJWTTokenSignalFull(OAIHttpRequestWorker *worker, OAIModel_auth_token summary);
    void createJWTTokenJSONSignalFull(OAIHttpRequestWorker *worker, OAIModel_auth_token summary);
    void createLDAPIdentityProviderSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void createOAuth2IdentityProviderSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void deleteIdentityProviderSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void fetchOAuth2TokenOnBehalfSignalFull(OAIHttpRequestWorker *worker, OAIForwarded_token_response summary);
    void invalidateJWTTokensSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void listAllIdentityProvidersSignalFull(OAIHttpRequestWorker *worker, QList<OAIIdentity_provider> summary);
    void listIdentityProvidersSignalFull(OAIHttpRequestWorker *worker, QList<OAIIdentity_provider> summary);
    void listLDAPIdentityProvidersSignalFull(OAIHttpRequestWorker *worker, OAIIdentity_provider_ldap summary);
    void listOAuth2IdentityProvidersSignalFull(OAIHttpRequestWorker *worker, QList<OAIIdentity_provider_oauth2_with_secret> summary);
    void loginWithLDAPSignalFull(OAIHttpRequestWorker *worker, OAIModel_auth_token summary);
    void updateLDAPIdentityProviderSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);
    void updateOAuth2IdentityProviderSignalFull(OAIHttpRequestWorker *worker, OAIMessage summary);

    void createJWTTokenSignalE(OAIModel_auth_token summary, QNetworkReply::NetworkError error_type, QString error_str);
    void createJWTTokenJSONSignalE(OAIModel_auth_token summary, QNetworkReply::NetworkError error_type, QString error_str);
    void createLDAPIdentityProviderSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void createOAuth2IdentityProviderSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void deleteIdentityProviderSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void fetchOAuth2TokenOnBehalfSignalE(OAIForwarded_token_response summary, QNetworkReply::NetworkError error_type, QString error_str);
    void invalidateJWTTokensSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void listAllIdentityProvidersSignalE(QList<OAIIdentity_provider> summary, QNetworkReply::NetworkError error_type, QString error_str);
    void listIdentityProvidersSignalE(QList<OAIIdentity_provider> summary, QNetworkReply::NetworkError error_type, QString error_str);
    void listLDAPIdentityProvidersSignalE(OAIIdentity_provider_ldap summary, QNetworkReply::NetworkError error_type, QString error_str);
    void listOAuth2IdentityProvidersSignalE(QList<OAIIdentity_provider_oauth2_with_secret> summary, QNetworkReply::NetworkError error_type, QString error_str);
    void loginWithLDAPSignalE(OAIModel_auth_token summary, QNetworkReply::NetworkError error_type, QString error_str);
    void updateLDAPIdentityProviderSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);
    void updateOAuth2IdentityProviderSignalE(OAIMessage summary, QNetworkReply::NetworkError error_type, QString error_str);

    void createJWTTokenSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void createJWTTokenJSONSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void createLDAPIdentityProviderSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void createOAuth2IdentityProviderSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void deleteIdentityProviderSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void fetchOAuth2TokenOnBehalfSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void invalidateJWTTokensSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void listAllIdentityProvidersSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void listIdentityProvidersSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void listLDAPIdentityProvidersSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void listOAuth2IdentityProvidersSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void loginWithLDAPSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void updateLDAPIdentityProviderSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void updateOAuth2IdentityProviderSignalEFull(OAIHttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);

    void abortRequestsSignal();
    void allPendingRequestsCompleted();

public slots:
    void tokenAvailable();
    
};

} // namespace OpenAPI
#endif
