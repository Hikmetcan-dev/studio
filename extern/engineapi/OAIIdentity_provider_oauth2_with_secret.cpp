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

#include "OAIIdentity_provider_oauth2_with_secret.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAIIdentity_provider_oauth2_with_secret::OAIIdentity_provider_oauth2_with_secret(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAIIdentity_provider_oauth2_with_secret::OAIIdentity_provider_oauth2_with_secret() {
    this->initializeModel();
}

OAIIdentity_provider_oauth2_with_secret::~OAIIdentity_provider_oauth2_with_secret() {}

void OAIIdentity_provider_oauth2_with_secret::initializeModel() {

    m_authorization_endpoint_isSet = false;
    m_authorization_endpoint_isValid = false;

    m_device_authorization_endpoint_isSet = false;
    m_device_authorization_endpoint_isValid = false;

    m_device_client_id_isSet = false;
    m_device_client_id_isValid = false;

    m_end_session_endpoint_isSet = false;
    m_end_session_endpoint_isValid = false;

    m_grant_types_supported_isSet = false;
    m_grant_types_supported_isValid = false;

    m_hidden_isSet = false;
    m_hidden_isValid = false;

    m_issuer_isSet = false;
    m_issuer_isValid = false;

    m_jwks_uri_isSet = false;
    m_jwks_uri_isValid = false;

    m_label_isSet = false;
    m_label_isValid = false;

    m_name_isSet = false;
    m_name_isValid = false;

    m_override_audience_isSet = false;
    m_override_audience_isValid = false;

    m_response_types_supported_isSet = false;
    m_response_types_supported_isValid = false;

    m_scopes_isSet = false;
    m_scopes_isValid = false;

    m_token_endpoint_isSet = false;
    m_token_endpoint_isValid = false;

    m_web_ui_client_id_isSet = false;
    m_web_ui_client_id_isValid = false;

    m_web_ui_client_secret_isSet = false;
    m_web_ui_client_secret_isValid = false;
}

void OAIIdentity_provider_oauth2_with_secret::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAIIdentity_provider_oauth2_with_secret::fromJsonObject(QJsonObject json) {

    m_authorization_endpoint_isValid = ::OpenAPI::fromJsonValue(authorization_endpoint, json[QString("authorization_endpoint")]);
    m_authorization_endpoint_isSet = !json[QString("authorization_endpoint")].isNull() && m_authorization_endpoint_isValid;

    m_device_authorization_endpoint_isValid = ::OpenAPI::fromJsonValue(device_authorization_endpoint, json[QString("device_authorization_endpoint")]);
    m_device_authorization_endpoint_isSet = !json[QString("device_authorization_endpoint")].isNull() && m_device_authorization_endpoint_isValid;

    m_device_client_id_isValid = ::OpenAPI::fromJsonValue(device_client_id, json[QString("device_client_id")]);
    m_device_client_id_isSet = !json[QString("device_client_id")].isNull() && m_device_client_id_isValid;

    m_end_session_endpoint_isValid = ::OpenAPI::fromJsonValue(end_session_endpoint, json[QString("end_session_endpoint")]);
    m_end_session_endpoint_isSet = !json[QString("end_session_endpoint")].isNull() && m_end_session_endpoint_isValid;

    m_grant_types_supported_isValid = ::OpenAPI::fromJsonValue(grant_types_supported, json[QString("grant_types_supported")]);
    m_grant_types_supported_isSet = !json[QString("grant_types_supported")].isNull() && m_grant_types_supported_isValid;

    m_hidden_isValid = ::OpenAPI::fromJsonValue(hidden, json[QString("hidden")]);
    m_hidden_isSet = !json[QString("hidden")].isNull() && m_hidden_isValid;

    m_issuer_isValid = ::OpenAPI::fromJsonValue(issuer, json[QString("issuer")]);
    m_issuer_isSet = !json[QString("issuer")].isNull() && m_issuer_isValid;

    m_jwks_uri_isValid = ::OpenAPI::fromJsonValue(jwks_uri, json[QString("jwks_uri")]);
    m_jwks_uri_isSet = !json[QString("jwks_uri")].isNull() && m_jwks_uri_isValid;

    m_label_isValid = ::OpenAPI::fromJsonValue(label, json[QString("label")]);
    m_label_isSet = !json[QString("label")].isNull() && m_label_isValid;

    m_name_isValid = ::OpenAPI::fromJsonValue(name, json[QString("name")]);
    m_name_isSet = !json[QString("name")].isNull() && m_name_isValid;

    m_override_audience_isValid = ::OpenAPI::fromJsonValue(override_audience, json[QString("override_audience")]);
    m_override_audience_isSet = !json[QString("override_audience")].isNull() && m_override_audience_isValid;

    m_response_types_supported_isValid = ::OpenAPI::fromJsonValue(response_types_supported, json[QString("response_types_supported")]);
    m_response_types_supported_isSet = !json[QString("response_types_supported")].isNull() && m_response_types_supported_isValid;

    m_scopes_isValid = ::OpenAPI::fromJsonValue(scopes, json[QString("scopes")]);
    m_scopes_isSet = !json[QString("scopes")].isNull() && m_scopes_isValid;

    m_token_endpoint_isValid = ::OpenAPI::fromJsonValue(token_endpoint, json[QString("token_endpoint")]);
    m_token_endpoint_isSet = !json[QString("token_endpoint")].isNull() && m_token_endpoint_isValid;

    m_web_ui_client_id_isValid = ::OpenAPI::fromJsonValue(web_ui_client_id, json[QString("web_ui_client_id")]);
    m_web_ui_client_id_isSet = !json[QString("web_ui_client_id")].isNull() && m_web_ui_client_id_isValid;

    m_web_ui_client_secret_isValid = ::OpenAPI::fromJsonValue(web_ui_client_secret, json[QString("web_ui_client_secret")]);
    m_web_ui_client_secret_isSet = !json[QString("web_ui_client_secret")].isNull() && m_web_ui_client_secret_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAIIdentity_provider_oauth2_with_secret::asJsonObject() const {
    QJsonObject obj;
    if (m_authorization_endpoint_isSet) {
        obj.insert(QString("authorization_endpoint"), ::OpenAPI::toJsonValue(authorization_endpoint));
    }
    if (m_device_authorization_endpoint_isSet) {
        obj.insert(QString("device_authorization_endpoint"), ::OpenAPI::toJsonValue(device_authorization_endpoint));
    }
    if (m_device_client_id_isSet) {
        obj.insert(QString("device_client_id"), ::OpenAPI::toJsonValue(device_client_id));
    }
    if (m_end_session_endpoint_isSet) {
        obj.insert(QString("end_session_endpoint"), ::OpenAPI::toJsonValue(end_session_endpoint));
    }
    if (grant_types_supported.size() > 0) {
        obj.insert(QString("grant_types_supported"), ::OpenAPI::toJsonValue(grant_types_supported));
    }
    if (m_hidden_isSet) {
        obj.insert(QString("hidden"), ::OpenAPI::toJsonValue(hidden));
    }
    if (m_issuer_isSet) {
        obj.insert(QString("issuer"), ::OpenAPI::toJsonValue(issuer));
    }
    if (m_jwks_uri_isSet) {
        obj.insert(QString("jwks_uri"), ::OpenAPI::toJsonValue(jwks_uri));
    }
    if (m_label_isSet) {
        obj.insert(QString("label"), ::OpenAPI::toJsonValue(label));
    }
    if (m_name_isSet) {
        obj.insert(QString("name"), ::OpenAPI::toJsonValue(name));
    }
    if (m_override_audience_isSet) {
        obj.insert(QString("override_audience"), ::OpenAPI::toJsonValue(override_audience));
    }
    if (response_types_supported.size() > 0) {
        obj.insert(QString("response_types_supported"), ::OpenAPI::toJsonValue(response_types_supported));
    }
    if (scopes.size() > 0) {
        obj.insert(QString("scopes"), ::OpenAPI::toJsonValue(scopes));
    }
    if (m_token_endpoint_isSet) {
        obj.insert(QString("token_endpoint"), ::OpenAPI::toJsonValue(token_endpoint));
    }
    if (m_web_ui_client_id_isSet) {
        obj.insert(QString("web_ui_client_id"), ::OpenAPI::toJsonValue(web_ui_client_id));
    }
    if (m_web_ui_client_secret_isSet) {
        obj.insert(QString("web_ui_client_secret"), ::OpenAPI::toJsonValue(web_ui_client_secret));
    }
    return obj;
}

QString OAIIdentity_provider_oauth2_with_secret::getAuthorizationEndpoint() const {
    return authorization_endpoint;
}
void OAIIdentity_provider_oauth2_with_secret::setAuthorizationEndpoint(const QString &authorization_endpoint) {
    this->authorization_endpoint = authorization_endpoint;
    this->m_authorization_endpoint_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_authorization_endpoint_Set() const{
    return m_authorization_endpoint_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_authorization_endpoint_Valid() const{
    return m_authorization_endpoint_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getDeviceAuthorizationEndpoint() const {
    return device_authorization_endpoint;
}
void OAIIdentity_provider_oauth2_with_secret::setDeviceAuthorizationEndpoint(const QString &device_authorization_endpoint) {
    this->device_authorization_endpoint = device_authorization_endpoint;
    this->m_device_authorization_endpoint_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_device_authorization_endpoint_Set() const{
    return m_device_authorization_endpoint_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_device_authorization_endpoint_Valid() const{
    return m_device_authorization_endpoint_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getDeviceClientId() const {
    return device_client_id;
}
void OAIIdentity_provider_oauth2_with_secret::setDeviceClientId(const QString &device_client_id) {
    this->device_client_id = device_client_id;
    this->m_device_client_id_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_device_client_id_Set() const{
    return m_device_client_id_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_device_client_id_Valid() const{
    return m_device_client_id_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getEndSessionEndpoint() const {
    return end_session_endpoint;
}
void OAIIdentity_provider_oauth2_with_secret::setEndSessionEndpoint(const QString &end_session_endpoint) {
    this->end_session_endpoint = end_session_endpoint;
    this->m_end_session_endpoint_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_end_session_endpoint_Set() const{
    return m_end_session_endpoint_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_end_session_endpoint_Valid() const{
    return m_end_session_endpoint_isValid;
}

QList<QString> OAIIdentity_provider_oauth2_with_secret::getGrantTypesSupported() const {
    return grant_types_supported;
}
void OAIIdentity_provider_oauth2_with_secret::setGrantTypesSupported(const QList<QString> &grant_types_supported) {
    this->grant_types_supported = grant_types_supported;
    this->m_grant_types_supported_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_grant_types_supported_Set() const{
    return m_grant_types_supported_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_grant_types_supported_Valid() const{
    return m_grant_types_supported_isValid;
}

bool OAIIdentity_provider_oauth2_with_secret::isHidden() const {
    return hidden;
}
void OAIIdentity_provider_oauth2_with_secret::setHidden(const bool &hidden) {
    this->hidden = hidden;
    this->m_hidden_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_hidden_Set() const{
    return m_hidden_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_hidden_Valid() const{
    return m_hidden_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getIssuer() const {
    return issuer;
}
void OAIIdentity_provider_oauth2_with_secret::setIssuer(const QString &issuer) {
    this->issuer = issuer;
    this->m_issuer_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_issuer_Set() const{
    return m_issuer_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_issuer_Valid() const{
    return m_issuer_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getJwksUri() const {
    return jwks_uri;
}
void OAIIdentity_provider_oauth2_with_secret::setJwksUri(const QString &jwks_uri) {
    this->jwks_uri = jwks_uri;
    this->m_jwks_uri_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_jwks_uri_Set() const{
    return m_jwks_uri_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_jwks_uri_Valid() const{
    return m_jwks_uri_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getLabel() const {
    return label;
}
void OAIIdentity_provider_oauth2_with_secret::setLabel(const QString &label) {
    this->label = label;
    this->m_label_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_label_Set() const{
    return m_label_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_label_Valid() const{
    return m_label_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getName() const {
    return name;
}
void OAIIdentity_provider_oauth2_with_secret::setName(const QString &name) {
    this->name = name;
    this->m_name_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_name_Set() const{
    return m_name_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_name_Valid() const{
    return m_name_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getOverrideAudience() const {
    return override_audience;
}
void OAIIdentity_provider_oauth2_with_secret::setOverrideAudience(const QString &override_audience) {
    this->override_audience = override_audience;
    this->m_override_audience_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_override_audience_Set() const{
    return m_override_audience_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_override_audience_Valid() const{
    return m_override_audience_isValid;
}

QList<QString> OAIIdentity_provider_oauth2_with_secret::getResponseTypesSupported() const {
    return response_types_supported;
}
void OAIIdentity_provider_oauth2_with_secret::setResponseTypesSupported(const QList<QString> &response_types_supported) {
    this->response_types_supported = response_types_supported;
    this->m_response_types_supported_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_response_types_supported_Set() const{
    return m_response_types_supported_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_response_types_supported_Valid() const{
    return m_response_types_supported_isValid;
}

QList<OAIIdentity_provider_oauth2_scope> OAIIdentity_provider_oauth2_with_secret::getScopes() const {
    return scopes;
}
void OAIIdentity_provider_oauth2_with_secret::setScopes(const QList<OAIIdentity_provider_oauth2_scope> &scopes) {
    this->scopes = scopes;
    this->m_scopes_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_scopes_Set() const{
    return m_scopes_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_scopes_Valid() const{
    return m_scopes_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getTokenEndpoint() const {
    return token_endpoint;
}
void OAIIdentity_provider_oauth2_with_secret::setTokenEndpoint(const QString &token_endpoint) {
    this->token_endpoint = token_endpoint;
    this->m_token_endpoint_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_token_endpoint_Set() const{
    return m_token_endpoint_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_token_endpoint_Valid() const{
    return m_token_endpoint_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getWebUiClientId() const {
    return web_ui_client_id;
}
void OAIIdentity_provider_oauth2_with_secret::setWebUiClientId(const QString &web_ui_client_id) {
    this->web_ui_client_id = web_ui_client_id;
    this->m_web_ui_client_id_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_web_ui_client_id_Set() const{
    return m_web_ui_client_id_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_web_ui_client_id_Valid() const{
    return m_web_ui_client_id_isValid;
}

QString OAIIdentity_provider_oauth2_with_secret::getWebUiClientSecret() const {
    return web_ui_client_secret;
}
void OAIIdentity_provider_oauth2_with_secret::setWebUiClientSecret(const QString &web_ui_client_secret) {
    this->web_ui_client_secret = web_ui_client_secret;
    this->m_web_ui_client_secret_isSet = true;
}

bool OAIIdentity_provider_oauth2_with_secret::is_web_ui_client_secret_Set() const{
    return m_web_ui_client_secret_isSet;
}

bool OAIIdentity_provider_oauth2_with_secret::is_web_ui_client_secret_Valid() const{
    return m_web_ui_client_secret_isValid;
}

bool OAIIdentity_provider_oauth2_with_secret::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_authorization_endpoint_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_device_authorization_endpoint_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_device_client_id_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_end_session_endpoint_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (grant_types_supported.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (m_hidden_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_issuer_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_jwks_uri_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_label_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_name_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_override_audience_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (response_types_supported.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (scopes.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (m_token_endpoint_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_web_ui_client_id_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_web_ui_client_secret_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAIIdentity_provider_oauth2_with_secret::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI
