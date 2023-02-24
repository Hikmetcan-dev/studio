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

/*
 * OAIHypercube_summary.h
 *
 * 
 */

#ifndef OAIHypercube_summary_H
#define OAIHypercube_summary_H

#include <QJsonObject>


#include "OAIEnum.h"
#include "OAIObject.h"

namespace OpenAPI {

class OAIHypercube_summary : public OAIObject {
public:
    OAIHypercube_summary();
    OAIHypercube_summary(QString json);
    ~OAIHypercube_summary() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    qint32 getFinished() const;
    void setFinished(const qint32 &finished);
    bool is_finished_Set() const;
    bool is_finished_Valid() const;

    qint32 getFinishedWithStatusCodeZero() const;
    void setFinishedWithStatusCodeZero(const qint32 &finished_with_status_code_zero);
    bool is_finished_with_status_code_zero_Set() const;
    bool is_finished_with_status_code_zero_Valid() const;

    qint32 getJobCount() const;
    void setJobCount(const qint32 &job_count);
    bool is_job_count_Set() const;
    bool is_job_count_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    qint32 finished;
    bool m_finished_isSet;
    bool m_finished_isValid;

    qint32 finished_with_status_code_zero;
    bool m_finished_with_status_code_zero_isSet;
    bool m_finished_with_status_code_zero_isValid;

    qint32 job_count;
    bool m_job_count_isSet;
    bool m_job_count_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::OAIHypercube_summary)

#endif // OAIHypercube_summary_H
