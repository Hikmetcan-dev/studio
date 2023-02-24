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
 * OAIModel_usage.h
 *
 * 
 */

#ifndef OAIModel_usage_H
#define OAIModel_usage_H

#include <QJsonObject>

#include "OAIModel_hypercube_usage.h"
#include "OAIModel_job_usage.h"
#include <QList>

#include "OAIEnum.h"
#include "OAIObject.h"

namespace OpenAPI {

class OAIModel_usage : public OAIObject {
public:
    OAIModel_usage();
    OAIModel_usage(QString json);
    ~OAIModel_usage() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QList<OAIModel_hypercube_usage> getHypercubeJobUsage() const;
    void setHypercubeJobUsage(const QList<OAIModel_hypercube_usage> &hypercube_job_usage);
    bool is_hypercube_job_usage_Set() const;
    bool is_hypercube_job_usage_Valid() const;

    QList<OAIModel_job_usage> getJobUsage() const;
    void setJobUsage(const QList<OAIModel_job_usage> &job_usage);
    bool is_job_usage_Set() const;
    bool is_job_usage_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QList<OAIModel_hypercube_usage> hypercube_job_usage;
    bool m_hypercube_job_usage_isSet;
    bool m_hypercube_job_usage_isValid;

    QList<OAIModel_job_usage> job_usage;
    bool m_job_usage_isSet;
    bool m_job_usage_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::OAIModel_usage)

#endif // OAIModel_usage_H
