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

#include "OAIInex.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAIInex::OAIInex(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAIInex::OAIInex() {
    this->initializeModel();
}

OAIInex::~OAIInex() {}

void OAIInex::initializeModel() {

    m_files_isSet = false;
    m_files_isValid = false;

    m_type_isSet = false;
    m_type_isValid = false;
}

void OAIInex::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAIInex::fromJsonObject(QJsonObject json) {

    m_files_isValid = ::OpenAPI::fromJsonValue(files, json[QString("files")]);
    m_files_isSet = !json[QString("files")].isNull() && m_files_isValid;

    m_type_isValid = ::OpenAPI::fromJsonValue(type, json[QString("type")]);
    m_type_isSet = !json[QString("type")].isNull() && m_type_isValid;
}

QString OAIInex::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAIInex::asJsonObject() const {
    QJsonObject obj;
    if (files.size() > 0) {
        obj.insert(QString("files"), ::OpenAPI::toJsonValue(files));
    }
    if (m_type_isSet) {
        obj.insert(QString("type"), ::OpenAPI::toJsonValue(type));
    }
    return obj;
}

QList<QString> OAIInex::getFiles() const {
    return files;
}
void OAIInex::setFiles(const QList<QString> &files) {
    this->files = files;
    this->m_files_isSet = true;
}

bool OAIInex::is_files_Set() const{
    return m_files_isSet;
}

bool OAIInex::is_files_Valid() const{
    return m_files_isValid;
}

QString OAIInex::getType() const {
    return type;
}
void OAIInex::setType(const QString &type) {
    this->type = type;
    this->m_type_isSet = true;
}

bool OAIInex::is_type_Set() const{
    return m_type_isSet;
}

bool OAIInex::is_type_Valid() const{
    return m_type_isValid;
}

bool OAIInex::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (files.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (m_type_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAIInex::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI
