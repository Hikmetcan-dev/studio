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
 * OAIResult_user.h
 *
 * 
 */

#ifndef OAIResult_user_H
#define OAIResult_user_H

#include <QJsonObject>

#include <QString>

#include "OAIEnum.h"
#include "OAIObject.h"

namespace OpenAPI {

class OAIResult_user : public OAIObject {
public:
    OAIResult_user();
    OAIResult_user(QString json);
    ~OAIResult_user() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    bool isDeleted() const;
    void setDeleted(const bool &deleted);
    bool is_deleted_Set() const;
    bool is_deleted_Valid() const;

    QString getOldUsername() const;
    void setOldUsername(const QString &old_username);
    bool is_old_username_Set() const;
    bool is_old_username_Valid() const;

    QString getUsername() const;
    void setUsername(const QString &username);
    bool is_username_Set() const;
    bool is_username_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    bool deleted;
    bool m_deleted_isSet;
    bool m_deleted_isValid;

    QString old_username;
    bool m_old_username_isSet;
    bool m_old_username_isValid;

    QString username;
    bool m_username_isSet;
    bool m_username_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::OAIResult_user)

#endif // OAIResult_user_H
