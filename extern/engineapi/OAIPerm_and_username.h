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
 * OAIPerm_and_username.h
 *
 * 
 */

#ifndef OAIPerm_and_username_H
#define OAIPerm_and_username_H

#include <QJsonObject>

#include <QString>

#include "OAIEnum.h"
#include "OAIObject.h"

namespace OpenAPI {

class OAIPerm_and_username : public OAIObject {
public:
    OAIPerm_and_username();
    OAIPerm_and_username(QString json);
    ~OAIPerm_and_username() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    qint32 getPermission() const;
    void setPermission(const qint32 &permission);
    bool is_permission_Set() const;
    bool is_permission_Valid() const;

    QString getUsername() const;
    void setUsername(const QString &username);
    bool is_username_Set() const;
    bool is_username_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    qint32 permission;
    bool m_permission_isSet;
    bool m_permission_isValid;

    QString username;
    bool m_username_isSet;
    bool m_username_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::OAIPerm_and_username)

#endif // OAIPerm_and_username_H
