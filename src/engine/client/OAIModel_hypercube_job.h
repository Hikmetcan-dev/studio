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

/*
 * OAIModel_hypercube_job.h
 *
 * 
 */

#ifndef OAIModel_hypercube_job_H
#define OAIModel_hypercube_job_H

#include <QJsonObject>

#include "OAITime_span.h"
#include <QList>

#include "OAIEnum.h"
#include "OAIObject.h"

namespace OpenAPI {

class OAIModel_hypercube_job : public OAIObject {
public:
    OAIModel_hypercube_job();
    OAIModel_hypercube_job(QString json);
    ~OAIModel_hypercube_job() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    qint32 getJobNumber() const;
    void setJobNumber(const qint32 &job_number);
    bool is_job_number_Set() const;
    bool is_job_number_Valid() const;

    qint32 getProcessStatus() const;
    void setProcessStatus(const qint32 &process_status);
    bool is_process_status_Set() const;
    bool is_process_status_Valid() const;

    qint32 getStatus() const;
    void setStatus(const qint32 &status);
    bool is_status_Set() const;
    bool is_status_Valid() const;

    QList<OAITime_span> getTimes() const;
    void setTimes(const QList<OAITime_span> &times);
    bool is_times_Set() const;
    bool is_times_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    qint32 job_number;
    bool m_job_number_isSet;
    bool m_job_number_isValid;

    qint32 process_status;
    bool m_process_status_isSet;
    bool m_process_status_isValid;

    qint32 status;
    bool m_status_isSet;
    bool m_status_isValid;

    QList<OAITime_span> times;
    bool m_times_isSet;
    bool m_times_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::OAIModel_hypercube_job)

#endif // OAIModel_hypercube_job_H
