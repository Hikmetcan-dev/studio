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

#include "OAIBad_input.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAIBad_input::OAIBad_input(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAIBad_input::OAIBad_input() {
    this->initializeModel();
}

OAIBad_input::~OAIBad_input() {}

void OAIBad_input::initializeModel() {

    m_errors_isSet = false;
    m_errors_isValid = false;

    m_message_isSet = false;
    m_message_isValid = false;
}

void OAIBad_input::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAIBad_input::fromJsonObject(QJsonObject json) {

    m_errors_isValid = ::OpenAPI::fromJsonValue(errors, json[QString("errors")]);
    m_errors_isSet = !json[QString("errors")].isNull() && m_errors_isValid;

    m_message_isValid = ::OpenAPI::fromJsonValue(message, json[QString("message")]);
    m_message_isSet = !json[QString("message")].isNull() && m_message_isValid;
}

QString OAIBad_input::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAIBad_input::asJsonObject() const {
    QJsonObject obj;
    if (errors.size() > 0) {
        obj.insert(QString("errors"), ::OpenAPI::toJsonValue(errors));
    }
    if (m_message_isSet) {
        obj.insert(QString("message"), ::OpenAPI::toJsonValue(message));
    }
    return obj;
}

QMap<QString, QString> OAIBad_input::getErrors() const {
    return errors;
}
void OAIBad_input::setErrors(const QMap<QString, QString> &errors) {
    this->errors = errors;
    this->m_errors_isSet = true;
}

bool OAIBad_input::is_errors_Set() const{
    return m_errors_isSet;
}

bool OAIBad_input::is_errors_Valid() const{
    return m_errors_isValid;
}

QString OAIBad_input::getMessage() const {
    return message;
}
void OAIBad_input::setMessage(const QString &message) {
    this->message = message;
    this->m_message_isSet = true;
}

bool OAIBad_input::is_message_Set() const{
    return m_message_isSet;
}

bool OAIBad_input::is_message_Valid() const{
    return m_message_isValid;
}

bool OAIBad_input::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (errors.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (m_message_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAIBad_input::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI
