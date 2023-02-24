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

#include "OAINot_found.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAINot_found::OAINot_found(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAINot_found::OAINot_found() {
    this->initializeModel();
}

OAINot_found::~OAINot_found() {}

void OAINot_found::initializeModel() {

    m_entity_isSet = false;
    m_entity_isValid = false;

    m_message_isSet = false;
    m_message_isValid = false;

    m_names_isSet = false;
    m_names_isValid = false;
}

void OAINot_found::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAINot_found::fromJsonObject(QJsonObject json) {

    m_entity_isValid = ::OpenAPI::fromJsonValue(entity, json[QString("entity")]);
    m_entity_isSet = !json[QString("entity")].isNull() && m_entity_isValid;

    m_message_isValid = ::OpenAPI::fromJsonValue(message, json[QString("message")]);
    m_message_isSet = !json[QString("message")].isNull() && m_message_isValid;

    m_names_isValid = ::OpenAPI::fromJsonValue(names, json[QString("names")]);
    m_names_isSet = !json[QString("names")].isNull() && m_names_isValid;
}

QString OAINot_found::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAINot_found::asJsonObject() const {
    QJsonObject obj;
    if (m_entity_isSet) {
        obj.insert(QString("entity"), ::OpenAPI::toJsonValue(entity));
    }
    if (m_message_isSet) {
        obj.insert(QString("message"), ::OpenAPI::toJsonValue(message));
    }
    if (names.size() > 0) {
        obj.insert(QString("names"), ::OpenAPI::toJsonValue(names));
    }
    return obj;
}

QString OAINot_found::getEntity() const {
    return entity;
}
void OAINot_found::setEntity(const QString &entity) {
    this->entity = entity;
    this->m_entity_isSet = true;
}

bool OAINot_found::is_entity_Set() const{
    return m_entity_isSet;
}

bool OAINot_found::is_entity_Valid() const{
    return m_entity_isValid;
}

QString OAINot_found::getMessage() const {
    return message;
}
void OAINot_found::setMessage(const QString &message) {
    this->message = message;
    this->m_message_isSet = true;
}

bool OAINot_found::is_message_Set() const{
    return m_message_isSet;
}

bool OAINot_found::is_message_Valid() const{
    return m_message_isValid;
}

QList<QString> OAINot_found::getNames() const {
    return names;
}
void OAINot_found::setNames(const QList<QString> &names) {
    this->names = names;
    this->m_names_isSet = true;
}

bool OAINot_found::is_names_Set() const{
    return m_names_isSet;
}

bool OAINot_found::is_names_Valid() const{
    return m_names_isValid;
}

bool OAINot_found::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_entity_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_message_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (names.size() > 0) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAINot_found::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI
