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

#include "OAIGeneric_key_value_pair.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAIGeneric_key_value_pair::OAIGeneric_key_value_pair(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAIGeneric_key_value_pair::OAIGeneric_key_value_pair() {
    this->initializeModel();
}

OAIGeneric_key_value_pair::~OAIGeneric_key_value_pair() {}

void OAIGeneric_key_value_pair::initializeModel() {

    m_key_isSet = false;
    m_key_isValid = false;

    m_value_isSet = false;
    m_value_isValid = false;
}

void OAIGeneric_key_value_pair::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAIGeneric_key_value_pair::fromJsonObject(QJsonObject json) {

    m_key_isValid = ::OpenAPI::fromJsonValue(key, json[QString("key")]);
    m_key_isSet = !json[QString("key")].isNull() && m_key_isValid;

    m_value_isValid = ::OpenAPI::fromJsonValue(value, json[QString("value")]);
    m_value_isSet = !json[QString("value")].isNull() && m_value_isValid;
}

QString OAIGeneric_key_value_pair::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAIGeneric_key_value_pair::asJsonObject() const {
    QJsonObject obj;
    if (m_key_isSet) {
        obj.insert(QString("key"), ::OpenAPI::toJsonValue(key));
    }
    if (m_value_isSet) {
        obj.insert(QString("value"), ::OpenAPI::toJsonValue(value));
    }
    return obj;
}

QString OAIGeneric_key_value_pair::getKey() const {
    return key;
}
void OAIGeneric_key_value_pair::setKey(const QString &key) {
    this->key = key;
    this->m_key_isSet = true;
}

bool OAIGeneric_key_value_pair::is_key_Set() const{
    return m_key_isSet;
}

bool OAIGeneric_key_value_pair::is_key_Valid() const{
    return m_key_isValid;
}

QString OAIGeneric_key_value_pair::getValue() const {
    return value;
}
void OAIGeneric_key_value_pair::setValue(const QString &value) {
    this->value = value;
    this->m_value_isSet = true;
}

bool OAIGeneric_key_value_pair::is_value_Set() const{
    return m_value_isSet;
}

bool OAIGeneric_key_value_pair::is_value_Valid() const{
    return m_value_isValid;
}

bool OAIGeneric_key_value_pair::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_key_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_value_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAIGeneric_key_value_pair::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI
