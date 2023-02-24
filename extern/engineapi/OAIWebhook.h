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
 * OAIWebhook.h
 *
 * 
 */

#ifndef OAIWebhook_H
#define OAIWebhook_H

#include <QJsonObject>

#include <QDateTime>
#include <QList>
#include <QString>

#include "OAIEnum.h"
#include "OAIObject.h"

namespace OpenAPI {

class OAIWebhook : public OAIObject {
public:
    OAIWebhook();
    OAIWebhook(QString json);
    ~OAIWebhook() override;

    QString asJson() const override;
    QJsonObject asJsonObject() const override;
    void fromJsonObject(QJsonObject json) override;
    void fromJson(QString jsonString) override;

    QString getContentType() const;
    void setContentType(const QString &content_type);
    bool is_content_type_Set() const;
    bool is_content_type_Valid() const;

    QDateTime getCreatedAt() const;
    void setCreatedAt(const QDateTime &created_at);
    bool is_created_at_Set() const;
    bool is_created_at_Valid() const;

    QList<QString> getEvents() const;
    void setEvents(const QList<QString> &events);
    bool is_events_Set() const;
    bool is_events_Valid() const;

    qint32 getId() const;
    void setId(const qint32 &id);
    bool is_id_Set() const;
    bool is_id_Valid() const;

    bool isInsecureSsl() const;
    void setInsecureSsl(const bool &insecure_ssl);
    bool is_insecure_ssl_Set() const;
    bool is_insecure_ssl_Valid() const;

    bool isRecursive() const;
    void setRecursive(const bool &recursive);
    bool is_recursive_Set() const;
    bool is_recursive_Valid() const;

    QString getUrl() const;
    void setUrl(const QString &url);
    bool is_url_Set() const;
    bool is_url_Valid() const;

    QString getUsername() const;
    void setUsername(const QString &username);
    bool is_username_Set() const;
    bool is_username_Valid() const;

    virtual bool isSet() const override;
    virtual bool isValid() const override;

private:
    void initializeModel();

    QString content_type;
    bool m_content_type_isSet;
    bool m_content_type_isValid;

    QDateTime created_at;
    bool m_created_at_isSet;
    bool m_created_at_isValid;

    QList<QString> events;
    bool m_events_isSet;
    bool m_events_isValid;

    qint32 id;
    bool m_id_isSet;
    bool m_id_isValid;

    bool insecure_ssl;
    bool m_insecure_ssl_isSet;
    bool m_insecure_ssl_isValid;

    bool recursive;
    bool m_recursive_isSet;
    bool m_recursive_isValid;

    QString url;
    bool m_url_isSet;
    bool m_url_isValid;

    QString username;
    bool m_username_isSet;
    bool m_username_isValid;
};

} // namespace OpenAPI

Q_DECLARE_METATYPE(OpenAPI::OAIWebhook)

#endif // OAIWebhook_H
