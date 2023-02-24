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
 * OAINamespace_quota.h
 *
 * 
 */

#ifndef OAINamespace_quota_H
#define OAINamespace_quota_H

#include <QJsonObject>

#include <QString>

#include "OAIEnum.h"
#include "OAIObject.h"

namespace OpenAPI {

class OAINamespace_quota : public OAIObject {
public:
    OAINamespace_quota();
    OAINamespace_quota(QString json);
    ~OAINamespace_quota() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    qint32 getDiskQuota() const;
    void setDiskQuota(const qint32 &disk_quota);
    bool is_disk_quota_Set() const;
    bool is_disk_quota_Valid() const;

    qint32 getDiskUse() const;
    void setDiskUse(const qint32 &disk_use);
    bool is_disk_use_Set() const;
    bool is_disk_use_Valid() const;

    QString getName() const;
    void setName(const QString &name);
    bool is_name_Set() const;
    bool is_name_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    qint32 disk_quota;
    bool m_disk_quota_isSet;
    bool m_disk_quota_isValid;

    qint32 disk_use;
    bool m_disk_use_isSet;
    bool m_disk_use_isValid;

    QString name;
    bool m_name_isSet;
    bool m_name_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::OAINamespace_quota)

#endif // OAINamespace_quota_H
