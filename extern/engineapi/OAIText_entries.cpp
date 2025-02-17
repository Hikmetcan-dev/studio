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

#include "OAIText_entries.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAIText_entries::OAIText_entries(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAIText_entries::OAIText_entries() {
    this->initializeModel();
}

OAIText_entries::~OAIText_entries() {}

void OAIText_entries::initializeModel() {

    m_text_entries_isSet = false;
    m_text_entries_isValid = false;
}

void OAIText_entries::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAIText_entries::fromJsonObject(QJsonObject json) {

    m_text_entries_isValid = ::OpenAPI::fromJsonValue(text_entries, json[QString("text_entries")]);
    m_text_entries_isSet = !json[QString("text_entries")].isNull() && m_text_entries_isValid;
}

QString OAIText_entries::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAIText_entries::asJsonObject() const {
    QJsonObject obj;
    if (text_entries.size() > 0) {
        obj.insert(QString("text_entries"), ::OpenAPI::toJsonValue(text_entries));
    }
    return obj;
}

QList<QString> OAIText_entries::getTextEntries() const {
    return text_entries;
}
void OAIText_entries::setTextEntries(const QList<QString> &text_entries) {
    this->text_entries = text_entries;
    this->m_text_entries_isSet = true;
}

bool OAIText_entries::is_text_entries_Set() const{
    return m_text_entries_isSet;
}

bool OAIText_entries::is_text_entries_Valid() const{
    return m_text_entries_isValid;
}

bool OAIText_entries::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (text_entries.size() > 0) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAIText_entries::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI
