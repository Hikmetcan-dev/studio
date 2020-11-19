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

#include "OAIResult_user.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAIResult_user::OAIResult_user(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAIResult_user::OAIResult_user() {
    this->initializeModel();
}

OAIResult_user::~OAIResult_user() {}

void OAIResult_user::initializeModel() {

    m_deleted_isSet = false;
    m_deleted_isValid = false;

    m_username_isSet = false;
    m_username_isValid = false;
}

void OAIResult_user::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAIResult_user::fromJsonObject(QJsonObject json) {

    m_deleted_isValid = ::OpenAPI::fromJsonValue(deleted, json[QString("deleted")]);
    m_deleted_isSet = !json[QString("deleted")].isNull() && m_deleted_isValid;

    m_username_isValid = ::OpenAPI::fromJsonValue(username, json[QString("username")]);
    m_username_isSet = !json[QString("username")].isNull() && m_username_isValid;
}

QString OAIResult_user::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAIResult_user::asJsonObject() const {
    QJsonObject obj;
    if (m_deleted_isSet) {
        obj.insert(QString("deleted"), ::OpenAPI::toJsonValue(deleted));
    }
    if (m_username_isSet) {
        obj.insert(QString("username"), ::OpenAPI::toJsonValue(username));
    }
    return obj;
}

bool OAIResult_user::isDeleted() const {
    return deleted;
}
void OAIResult_user::setDeleted(const bool &deleted) {
    this->deleted = deleted;
    this->m_deleted_isSet = true;
}

bool OAIResult_user::is_deleted_Set() const{
    return m_deleted_isSet;
}

bool OAIResult_user::is_deleted_Valid() const{
    return m_deleted_isValid;
}

QString OAIResult_user::getUsername() const {
    return username;
}
void OAIResult_user::setUsername(const QString &username) {
    this->username = username;
    this->m_username_isSet = true;
}

bool OAIResult_user::is_username_Set() const{
    return m_username_isSet;
}

bool OAIResult_user::is_username_Valid() const{
    return m_username_isValid;
}

bool OAIResult_user::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_deleted_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_username_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAIResult_user::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI