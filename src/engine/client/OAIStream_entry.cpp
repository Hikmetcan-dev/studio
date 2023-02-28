/**
 * GAMS Engine
 * With GAMS Engine you can register and solve GAMS models. It has a namespace management system, so you can restrict your users to certain models.
 *
 * The version of the OpenAPI document: latest
 *
 * NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

#include "OAIStream_entry.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAIStream_entry::OAIStream_entry(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAIStream_entry::OAIStream_entry() {
    this->initializeModel();
}

OAIStream_entry::~OAIStream_entry() {}

void OAIStream_entry::initializeModel() {

    m_entry_value_isSet = false;
    m_entry_value_isValid = false;
}

void OAIStream_entry::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAIStream_entry::fromJsonObject(QJsonObject json) {

    m_entry_value_isValid = ::OpenAPI::fromJsonValue(entry_value, json[QString("entry_value")]);
    m_entry_value_isSet = !json[QString("entry_value")].isNull() && m_entry_value_isValid;
}

QString OAIStream_entry::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAIStream_entry::asJsonObject() const {
    QJsonObject obj;
    if (m_entry_value_isSet) {
        obj.insert(QString("entry_value"), ::OpenAPI::toJsonValue(entry_value));
    }
    return obj;
}

QString OAIStream_entry::getEntryValue() const {
    return entry_value;
}
void OAIStream_entry::setEntryValue(const QString &entry_value) {
    this->entry_value = entry_value;
    this->m_entry_value_isSet = true;
}

bool OAIStream_entry::is_entry_value_Set() const{
    return m_entry_value_isSet;
}

bool OAIStream_entry::is_entry_value_Valid() const{
    return m_entry_value_isValid;
}

bool OAIStream_entry::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_entry_value_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAIStream_entry::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI