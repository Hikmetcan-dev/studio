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

#include "OAIMessage_and_token.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAIMessage_and_token::OAIMessage_and_token(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAIMessage_and_token::OAIMessage_and_token() {
    this->initializeModel();
}

OAIMessage_and_token::~OAIMessage_and_token() {}

void OAIMessage_and_token::initializeModel() {

    m_message_isSet = false;
    m_message_isValid = false;

    m_quota_warning_isSet = false;
    m_quota_warning_isValid = false;

    m_token_isSet = false;
    m_token_isValid = false;
}

void OAIMessage_and_token::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAIMessage_and_token::fromJsonObject(QJsonObject json) {

    m_message_isValid = ::OpenAPI::fromJsonValue(message, json[QString("message")]);
    m_message_isSet = !json[QString("message")].isNull() && m_message_isValid;

    m_quota_warning_isValid = ::OpenAPI::fromJsonValue(quota_warning, json[QString("quota_warning")]);
    m_quota_warning_isSet = !json[QString("quota_warning")].isNull() && m_quota_warning_isValid;

    m_token_isValid = ::OpenAPI::fromJsonValue(token, json[QString("token")]);
    m_token_isSet = !json[QString("token")].isNull() && m_token_isValid;
}

QString OAIMessage_and_token::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAIMessage_and_token::asJsonObject() const {
    QJsonObject obj;
    if (m_message_isSet) {
        obj.insert(QString("message"), ::OpenAPI::toJsonValue(message));
    }
    if (quota_warning.size() > 0) {
        obj.insert(QString("quota_warning"), ::OpenAPI::toJsonValue(quota_warning));
    }
    if (m_token_isSet) {
        obj.insert(QString("token"), ::OpenAPI::toJsonValue(token));
    }
    return obj;
}

QString OAIMessage_and_token::getMessage() const {
    return message;
}
void OAIMessage_and_token::setMessage(const QString &message) {
    this->message = message;
    this->m_message_isSet = true;
}

bool OAIMessage_and_token::is_message_Set() const{
    return m_message_isSet;
}

bool OAIMessage_and_token::is_message_Valid() const{
    return m_message_isValid;
}

QList<OAIQuota> OAIMessage_and_token::getQuotaWarning() const {
    return quota_warning;
}
void OAIMessage_and_token::setQuotaWarning(const QList<OAIQuota> &quota_warning) {
    this->quota_warning = quota_warning;
    this->m_quota_warning_isSet = true;
}

bool OAIMessage_and_token::is_quota_warning_Set() const{
    return m_quota_warning_isSet;
}

bool OAIMessage_and_token::is_quota_warning_Valid() const{
    return m_quota_warning_isValid;
}

QString OAIMessage_and_token::getToken() const {
    return token;
}
void OAIMessage_and_token::setToken(const QString &token) {
    this->token = token;
    this->m_token_isSet = true;
}

bool OAIMessage_and_token::is_token_Set() const{
    return m_token_isSet;
}

bool OAIMessage_and_token::is_token_Valid() const{
    return m_token_isValid;
}

bool OAIMessage_and_token::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_message_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (quota_warning.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (m_token_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAIMessage_and_token::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI
