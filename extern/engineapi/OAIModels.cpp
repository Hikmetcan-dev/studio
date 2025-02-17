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

#include "OAIModels.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QObject>

#include "OAIHelpers.h"

namespace OpenAPI {

OAIModels::OAIModels(QString json) {
    this->initializeModel();
    this->fromJson(json);
}

OAIModels::OAIModels() {
    this->initializeModel();
}

OAIModels::~OAIModels() {}

void OAIModels::initializeModel() {

    m_arguments_isSet = false;
    m_arguments_isValid = false;

    m_inex_isSet = false;
    m_inex_isValid = false;

    m_length_isSet = false;
    m_length_isValid = false;

    m_name_isSet = false;
    m_name_isValid = false;

    m_protect_model_files_isSet = false;
    m_protect_model_files_isValid = false;

    m_run_isSet = false;
    m_run_isValid = false;

    m_stream_entries_isSet = false;
    m_stream_entries_isValid = false;

    m_text_entries_isSet = false;
    m_text_entries_isValid = false;

    m_upload_date_isSet = false;
    m_upload_date_isValid = false;

    m_user_groups_isSet = false;
    m_user_groups_isValid = false;
}

void OAIModels::fromJson(QString jsonString) {
    QByteArray array(jsonString.toStdString().c_str());
    QJsonDocument doc = QJsonDocument::fromJson(array);
    QJsonObject jsonObject = doc.object();
    this->fromJsonObject(jsonObject);
}

void OAIModels::fromJsonObject(QJsonObject json) {

    m_arguments_isValid = ::OpenAPI::fromJsonValue(arguments, json[QString("arguments")]);
    m_arguments_isSet = !json[QString("arguments")].isNull() && m_arguments_isValid;

    m_inex_isValid = ::OpenAPI::fromJsonValue(inex, json[QString("inex")]);
    m_inex_isSet = !json[QString("inex")].isNull() && m_inex_isValid;

    m_length_isValid = ::OpenAPI::fromJsonValue(length, json[QString("length")]);
    m_length_isSet = !json[QString("length")].isNull() && m_length_isValid;

    m_name_isValid = ::OpenAPI::fromJsonValue(name, json[QString("name")]);
    m_name_isSet = !json[QString("name")].isNull() && m_name_isValid;

    m_protect_model_files_isValid = ::OpenAPI::fromJsonValue(protect_model_files, json[QString("protect_model_files")]);
    m_protect_model_files_isSet = !json[QString("protect_model_files")].isNull() && m_protect_model_files_isValid;

    m_run_isValid = ::OpenAPI::fromJsonValue(run, json[QString("run")]);
    m_run_isSet = !json[QString("run")].isNull() && m_run_isValid;

    m_stream_entries_isValid = ::OpenAPI::fromJsonValue(stream_entries, json[QString("stream_entries")]);
    m_stream_entries_isSet = !json[QString("stream_entries")].isNull() && m_stream_entries_isValid;

    m_text_entries_isValid = ::OpenAPI::fromJsonValue(text_entries, json[QString("text_entries")]);
    m_text_entries_isSet = !json[QString("text_entries")].isNull() && m_text_entries_isValid;

    m_upload_date_isValid = ::OpenAPI::fromJsonValue(upload_date, json[QString("upload_date")]);
    m_upload_date_isSet = !json[QString("upload_date")].isNull() && m_upload_date_isValid;

    m_user_groups_isValid = ::OpenAPI::fromJsonValue(user_groups, json[QString("user_groups")]);
    m_user_groups_isSet = !json[QString("user_groups")].isNull() && m_user_groups_isValid;
}

QString OAIModels::asJson() const {
    QJsonObject obj = this->asJsonObject();
    QJsonDocument doc(obj);
    QByteArray bytes = doc.toJson();
    return QString(bytes);
}

QJsonObject OAIModels::asJsonObject() const {
    QJsonObject obj;
    if (arguments.size() > 0) {
        obj.insert(QString("arguments"), ::OpenAPI::toJsonValue(arguments));
    }
    if (inex.isSet()) {
        obj.insert(QString("inex"), ::OpenAPI::toJsonValue(inex));
    }
    if (m_length_isSet) {
        obj.insert(QString("length"), ::OpenAPI::toJsonValue(length));
    }
    if (m_name_isSet) {
        obj.insert(QString("name"), ::OpenAPI::toJsonValue(name));
    }
    if (m_protect_model_files_isSet) {
        obj.insert(QString("protect_model_files"), ::OpenAPI::toJsonValue(protect_model_files));
    }
    if (m_run_isSet) {
        obj.insert(QString("run"), ::OpenAPI::toJsonValue(run));
    }
    if (stream_entries.size() > 0) {
        obj.insert(QString("stream_entries"), ::OpenAPI::toJsonValue(stream_entries));
    }
    if (text_entries.size() > 0) {
        obj.insert(QString("text_entries"), ::OpenAPI::toJsonValue(text_entries));
    }
    if (m_upload_date_isSet) {
        obj.insert(QString("upload_date"), ::OpenAPI::toJsonValue(upload_date));
    }
    if (user_groups.size() > 0) {
        obj.insert(QString("user_groups"), ::OpenAPI::toJsonValue(user_groups));
    }
    return obj;
}

QList<QString> OAIModels::getArguments() const {
    return arguments;
}
void OAIModels::setArguments(const QList<QString> &arguments) {
    this->arguments = arguments;
    this->m_arguments_isSet = true;
}

bool OAIModels::is_arguments_Set() const{
    return m_arguments_isSet;
}

bool OAIModels::is_arguments_Valid() const{
    return m_arguments_isValid;
}

OAIInex OAIModels::getInex() const {
    return inex;
}
void OAIModels::setInex(const OAIInex &inex) {
    this->inex = inex;
    this->m_inex_isSet = true;
}

bool OAIModels::is_inex_Set() const{
    return m_inex_isSet;
}

bool OAIModels::is_inex_Valid() const{
    return m_inex_isValid;
}

qint32 OAIModels::getLength() const {
    return length;
}
void OAIModels::setLength(const qint32 &length) {
    this->length = length;
    this->m_length_isSet = true;
}

bool OAIModels::is_length_Set() const{
    return m_length_isSet;
}

bool OAIModels::is_length_Valid() const{
    return m_length_isValid;
}

QString OAIModels::getName() const {
    return name;
}
void OAIModels::setName(const QString &name) {
    this->name = name;
    this->m_name_isSet = true;
}

bool OAIModels::is_name_Set() const{
    return m_name_isSet;
}

bool OAIModels::is_name_Valid() const{
    return m_name_isValid;
}

bool OAIModels::isProtectModelFiles() const {
    return protect_model_files;
}
void OAIModels::setProtectModelFiles(const bool &protect_model_files) {
    this->protect_model_files = protect_model_files;
    this->m_protect_model_files_isSet = true;
}

bool OAIModels::is_protect_model_files_Set() const{
    return m_protect_model_files_isSet;
}

bool OAIModels::is_protect_model_files_Valid() const{
    return m_protect_model_files_isValid;
}

QString OAIModels::getRun() const {
    return run;
}
void OAIModels::setRun(const QString &run) {
    this->run = run;
    this->m_run_isSet = true;
}

bool OAIModels::is_run_Set() const{
    return m_run_isSet;
}

bool OAIModels::is_run_Valid() const{
    return m_run_isValid;
}

QList<QString> OAIModels::getStreamEntries() const {
    return stream_entries;
}
void OAIModels::setStreamEntries(const QList<QString> &stream_entries) {
    this->stream_entries = stream_entries;
    this->m_stream_entries_isSet = true;
}

bool OAIModels::is_stream_entries_Set() const{
    return m_stream_entries_isSet;
}

bool OAIModels::is_stream_entries_Valid() const{
    return m_stream_entries_isValid;
}

QList<QString> OAIModels::getTextEntries() const {
    return text_entries;
}
void OAIModels::setTextEntries(const QList<QString> &text_entries) {
    this->text_entries = text_entries;
    this->m_text_entries_isSet = true;
}

bool OAIModels::is_text_entries_Set() const{
    return m_text_entries_isSet;
}

bool OAIModels::is_text_entries_Valid() const{
    return m_text_entries_isValid;
}

QDateTime OAIModels::getUploadDate() const {
    return upload_date;
}
void OAIModels::setUploadDate(const QDateTime &upload_date) {
    this->upload_date = upload_date;
    this->m_upload_date_isSet = true;
}

bool OAIModels::is_upload_date_Set() const{
    return m_upload_date_isSet;
}

bool OAIModels::is_upload_date_Valid() const{
    return m_upload_date_isValid;
}

QList<QString> OAIModels::getUserGroups() const {
    return user_groups;
}
void OAIModels::setUserGroups(const QList<QString> &user_groups) {
    this->user_groups = user_groups;
    this->m_user_groups_isSet = true;
}

bool OAIModels::is_user_groups_Set() const{
    return m_user_groups_isSet;
}

bool OAIModels::is_user_groups_Valid() const{
    return m_user_groups_isValid;
}

bool OAIModels::isSet() const {
    bool isObjectUpdated = false;
    do {
        if (arguments.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (inex.isSet()) {
            isObjectUpdated = true;
            break;
        }

        if (m_length_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_name_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_protect_model_files_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (m_run_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (stream_entries.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (text_entries.size() > 0) {
            isObjectUpdated = true;
            break;
        }

        if (m_upload_date_isSet) {
            isObjectUpdated = true;
            break;
        }

        if (user_groups.size() > 0) {
            isObjectUpdated = true;
            break;
        }
    } while (false);
    return isObjectUpdated;
}

bool OAIModels::isValid() const {
    // only required properties are required for the object to be considered valid
    return true;
}

} // namespace OpenAPI
