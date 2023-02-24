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

#include "OAINamespace_quota.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAINamespace_quota::OAINamespace_quota(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAINamespace_quota::OAINamespace_quota() {
    this->initializeModel();
}

OAINamespace_quota::~OAINamespace_quota() {}

void OAINamespace_quota::initializeModel() {

    m_disk_quota_isSet = false;
    m_disk_quota_isValid = false;

    m_disk_use_isSet = false;
    m_disk_use_isValid = false;

    m_name_isSet = false;
    m_name_isValid = false;
}

void OAINamespace_quota::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAINamespace_quota::fromJsonObject(QJsonObject json) {

    m_disk_quota_isValid = ::OpenAPI::fromJsonValue(disk_quota, json[QString("disk_quota")]);
    m_disk_quota_isSet = !json[QString("disk_quota")].isNull() && m_disk_quota_isValid;

    m_disk_use_isValid = ::OpenAPI::fromJsonValue(disk_use, json[QString("disk_use")]);
    m_disk_use_isSet = !json[QString("disk_use")].isNull() && m_disk_use_isValid;

    m_name_isValid = ::OpenAPI::fromJsonValue(name, json[QString("name")]);
    m_name_isSet = !json[QString("name")].isNull() && m_name_isValid;
}

QString OAINamespace_quota::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAINamespace_quota::asJsonObject() const {
    QJsonObject obj;
    if (m_disk_quota_isSet) {
        obj.insert(QString("disk_quota"), ::OpenAPI::toJsonValue(disk_quota));
    }
    if (m_disk_use_isSet) {
        obj.insert(QString("disk_use"), ::OpenAPI::toJsonValue(disk_use));
    }
    if (m_name_isSet) {
        obj.insert(QString("name"), ::OpenAPI::toJsonValue(name));
    }
    return obj;
}

qint32 OAINamespace_quota::getDiskQuota() const {
    return disk_quota;
}
void OAINamespace_quota::setDiskQuota(const qint32 &disk_quota) {
    this->disk_quota = disk_quota;
    this->m_disk_quota_isSet = true;
}

bool OAINamespace_quota::is_disk_quota_Set() const{
    return m_disk_quota_isSet;
}

bool OAINamespace_quota::is_disk_quota_Valid() const{
    return m_disk_quota_isValid;
}

qint32 OAINamespace_quota::getDiskUse() const {
    return disk_use;
}
void OAINamespace_quota::setDiskUse(const qint32 &disk_use) {
    this->disk_use = disk_use;
    this->m_disk_use_isSet = true;
}

bool OAINamespace_quota::is_disk_use_Set() const{
    return m_disk_use_isSet;
}

bool OAINamespace_quota::is_disk_use_Valid() const{
    return m_disk_use_isValid;
}

QString OAINamespace_quota::getName() const {
    return name;
}
void OAINamespace_quota::setName(const QString &name) {
    this->name = name;
    this->m_name_isSet = true;
}

bool OAINamespace_quota::is_name_Set() const{
    return m_name_isSet;
}

bool OAINamespace_quota::is_name_Valid() const{
    return m_name_isValid;
}

bool OAINamespace_quota::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_disk_quota_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_disk_use_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_name_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAINamespace_quota::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI
