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

#include "OAIModel_hypercube_job.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAIModel_hypercube_job::OAIModel_hypercube_job(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAIModel_hypercube_job::OAIModel_hypercube_job() {
    this->initializeModel();
}

OAIModel_hypercube_job::~OAIModel_hypercube_job() {}

void OAIModel_hypercube_job::initializeModel() {

    m_job_number_isSet = false;
    m_job_number_isValid = false;

    m_process_status_isSet = false;
    m_process_status_isValid = false;

    m_status_isSet = false;
    m_status_isValid = false;

    m_times_isSet = false;
    m_times_isValid = false;
}

void OAIModel_hypercube_job::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAIModel_hypercube_job::fromJsonObject(QJsonObject json) {

    m_job_number_isValid = ::OpenAPI::fromJsonValue(job_number, json[QString("job_number")]);
    m_job_number_isSet = !json[QString("job_number")].isNull() && m_job_number_isValid;

    m_process_status_isValid = ::OpenAPI::fromJsonValue(process_status, json[QString("process_status")]);
    m_process_status_isSet = !json[QString("process_status")].isNull() && m_process_status_isValid;

    m_status_isValid = ::OpenAPI::fromJsonValue(status, json[QString("status")]);
    m_status_isSet = !json[QString("status")].isNull() && m_status_isValid;

    m_times_isValid = ::OpenAPI::fromJsonValue(times, json[QString("times")]);
    m_times_isSet = !json[QString("times")].isNull() && m_times_isValid;
}

QString OAIModel_hypercube_job::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAIModel_hypercube_job::asJsonObject() const {
    QJsonObject obj;
    if (m_job_number_isSet) {
        obj.insert(QString("job_number"), ::OpenAPI::toJsonValue(job_number));
    }
    if (m_process_status_isSet) {
        obj.insert(QString("process_status"), ::OpenAPI::toJsonValue(process_status));
    }
    if (m_status_isSet) {
        obj.insert(QString("status"), ::OpenAPI::toJsonValue(status));
    }
    if (times.size() > 0) {
        obj.insert(QString("times"), ::OpenAPI::toJsonValue(times));
    }
    return obj;
}

qint32 OAIModel_hypercube_job::getJobNumber() const {
    return job_number;
}
void OAIModel_hypercube_job::setJobNumber(const qint32 &job_number) {
    this->job_number = job_number;
    this->m_job_number_isSet = true;
}

bool OAIModel_hypercube_job::is_job_number_Set() const{
    return m_job_number_isSet;
}

bool OAIModel_hypercube_job::is_job_number_Valid() const{
    return m_job_number_isValid;
}

qint32 OAIModel_hypercube_job::getProcessStatus() const {
    return process_status;
}
void OAIModel_hypercube_job::setProcessStatus(const qint32 &process_status) {
    this->process_status = process_status;
    this->m_process_status_isSet = true;
}

bool OAIModel_hypercube_job::is_process_status_Set() const{
    return m_process_status_isSet;
}

bool OAIModel_hypercube_job::is_process_status_Valid() const{
    return m_process_status_isValid;
}

qint32 OAIModel_hypercube_job::getStatus() const {
    return status;
}
void OAIModel_hypercube_job::setStatus(const qint32 &status) {
    this->status = status;
    this->m_status_isSet = true;
}

bool OAIModel_hypercube_job::is_status_Set() const{
    return m_status_isSet;
}

bool OAIModel_hypercube_job::is_status_Valid() const{
    return m_status_isValid;
}

QList<OAITime_span> OAIModel_hypercube_job::getTimes() const {
    return times;
}
void OAIModel_hypercube_job::setTimes(const QList<OAITime_span> &times) {
    this->times = times;
    this->m_times_isSet = true;
}

bool OAIModel_hypercube_job::is_times_Set() const{
    return m_times_isSet;
}

bool OAIModel_hypercube_job::is_times_Valid() const{
    return m_times_isValid;
}

bool OAIModel_hypercube_job::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (m_job_number_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_process_status_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_status_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (times.size() > 0) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAIModel_hypercube_job::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI