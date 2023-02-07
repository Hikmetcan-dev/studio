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

#include "OAITime_span.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAITime_span::OAITime_span(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAITime_span::OAITime_span() {
    this->initializeModel();
}

OAITime_span::~OAITime_span() {}

void OAITime_span::initializeModel() {

    m_finish_isSet = false;
    m_finish_isValid = false;

    m_start_isSet = false;
    m_start_isValid = false;
}

void OAITime_span::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAITime_span::fromJsonObject(QJsonObject json) {

    m_finish_isValid = ::OpenAPI::fromJsonValue(finish, json[QString("finish")]);
    m_finish_isSet = !json[QString("finish")].isNull() && m_finish_isValid;

    m_start_isValid = ::OpenAPI::fromJsonValue(start, json[QString("start")]);
    m_start_isSet = !json[QString("start")].isNull() && m_start_isValid;
}

QString OAITime_span::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAITime_span::asJsonObject() const {
    QJsonObject obj;
    if (m_finish_isSet) {
        obj.insert(QString("finish"), ::OpenAPI::toJsonValue(finish));
    }
    if (m_start_isSet) {
        obj.insert(QString("start"), ::OpenAPI::toJsonValue(start));
    }
    return obj;
}

QDateTime OAITime_span::getFinish() const {
    return finish;
}
void OAITime_span::setFinish(const QDateTime &finish) {
    this->finish = finish;
    this->m_finish_isSet = true;
}

bool OAITime_span::is_finish_Set() const{
    return m_finish_isSet;
}

bool OAITime_span::is_finish_Valid() const{
    return m_finish_isValid;
}

QDateTime OAITime_span::getStart() const {
    return start;
}
void OAITime_span::setStart(const QDateTime &start) {
    this->start = start;
    this->m_start_isSet = true;
}

bool OAITime_span::is_start_Set() const{
    return m_start_isSet;
}

bool OAITime_span::is_start_Valid() const{
    return m_start_isValid;
}

bool OAITime_span::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_finish_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_start_isSet) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAITime_span::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI