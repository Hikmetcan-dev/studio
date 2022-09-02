/*
 * This file is part of the GAMS Studio project.
 *
 * Copyright (c) 2017-2022 GAMS Software GmbH <support@gams.com>
 * Copyright (c) 2017-2022 GAMS Development Corp. <support@gams.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <QDebug>
#include "connectschema.h"

namespace gams {
namespace studio {
namespace connect {


void ConnectSchema::createSchemaHelper(QString& key, const YAML::Node& node, int level)
{
    QList<SchemaType> types;
    if (node["type"].Type()==YAML::NodeType::Sequence) {
        for(size_t i=0; i<node["type"].size(); i++) {
           QString value(QString::fromStdString(node["type"][i].as<std::string>()));
           if (node["type"][i])
               types << getTypeFromValue(value);
        }
    } else if (node["type"].Type()==YAML::NodeType::Scalar) {
              QString value(QString::fromStdString(node["type"].as<std::string>()));
              if (node["type"])
                  types << getTypeFromValue(value);
    }
    bool required = false;
    if (node["required"]) {
        required = (node["required"].as<std::string>().compare("true") == 0 || node["required"].as<std::string>().compare("True") == 0);
    }
    QList<ValueWrapper> allowedValues;
    if (node["allowed"] && node.Type() == YAML::NodeType::Map) {
        for(size_t i=0; i<node["allowed"].size(); i++) {
            allowedValues <<  ValueWrapper(node["allowed"][i].as<std::string>());
        }
    }
    ValueWrapper defvalue;
    if (node["default"] ) {
        if (std::find(types.begin(), types.end(), SchemaType::Integer) != types.end()) {
            defvalue = ValueWrapper(node["default"].as<int>());
        } else if (std::find(types.begin(), types.end(), SchemaType::Float) != types.end()) {
                  defvalue = ValueWrapper(node["default"].as<double>());
        } else if (std::find(types.begin(), types.end(), SchemaType::String) != types.end()) {
            defvalue = ValueWrapper(node["default"].as<std::string>());
        } else if (std::find(types.begin(), types.end(), SchemaType::Boolean) != types.end()) {
            defvalue = ValueWrapper(node["default"].as<bool>());
        }
    }
    ValueWrapper minvalue;
    if (node["min"] ) {
        if (std::find(types.begin(), types.end(), SchemaType::Integer) != types.end()) {
            minvalue = ValueWrapper(node["min"].as<int>());
        } else if (std::find(types.begin(), types.end(), SchemaType::Float) != types.end()) {
                  minvalue = ValueWrapper(node["min"].as<double>());
        }
    }
    ValueWrapper maxvalue;
    if (node["max"] ) {
        if (std::find(types.begin(), types.end(), SchemaType::Integer) != types.end()) {
            maxvalue = ValueWrapper(node["max"].as<int>());
        } else if (std::find(types.begin(), types.end(), SchemaType::Float) != types.end()) {
                  maxvalue = ValueWrapper(node["max"].as<double>());
        }
    }
    mOrderedKeyList << key;

    bool schemaDefined = (node["schema"] ? true : false);
    Schema* s = new Schema(level, node, types, required, allowedValues, defvalue, minvalue, maxvalue, schemaDefined);
    mSchemaHelper.insert(key, s);
    if (node["schema"]) {
        if (mSchemaHelper[key]->hasType(SchemaType::List)) {
            QString str = key + ":-";
            createSchemaHelper(str, node["schema"], ++level);
        } else if (mSchemaHelper[key]->hasType(SchemaType::Dict)) {
                  ++level;
                  QString str;
                  for (YAML::const_iterator it=node["schema"].begin(); it != node["schema"].end(); ++it) {
                      str = key + ":";
                      if (it->second.Type() == YAML::NodeType::Map) {
                          str += QString::fromStdString( it->first.as<std::string>() );
                          createSchemaHelper(str, it->second, level);
                      }
                    }
        } // else  'schema' with niether 'type:list' nor 'type:dict' ?
    }
}

ConnectSchema::ConnectSchema(const QString &inputFileName)
{
    loadFromFile(inputFileName);
}

ConnectSchema::~ConnectSchema()
{
    if (mSchemaHelper.size()>0) {
       qDeleteAll( mSchemaHelper );
       mSchemaHelper.clear();
    }
}

void ConnectSchema::loadFromFile(const QString &inputFileName)
{
    ConnectAgent::loadFromFile(inputFileName);
    Q_ASSERT(mRootNode.Type()==YAML::NodeType::Map);
    for (YAML::const_iterator it = mRootNode.begin(); it != mRootNode.end(); ++it) {
        QString key( QString::fromStdString( it->first.as<std::string>() ) );
        createSchemaHelper(key, it->second, 1);
    }
}

void ConnectSchema::loadFromString(const QString &input)
{
    ConnectAgent::loadFromString(input);

    for (YAML::const_iterator it = mRootNode.begin(); it != mRootNode.end(); ++it) {
        QString key( QString::fromStdString( it->first.as<std::string>() ) );
        createSchemaHelper(key, it->second, 1);
    }

}

QStringList ConnectSchema::getlKeyList() const
{
    return mOrderedKeyList;
}

QStringList ConnectSchema::getFirstLevelKeyList() const
{
    QStringList keyList;
    for(const QString& key : mOrderedKeyList) {
        if (!key.contains(":"))
            keyList << key;
    }
    return keyList;
}

QStringList ConnectSchema::getNextLevelKeyList(const QString& key) const
{
    QStringList keyList;
    for(const QString& k : mOrderedKeyList) {
        if (k.startsWith(key+":"))
            keyList << k;
    }
    return keyList;
}

QStringList ConnectSchema::getAllRequiredKeyList() const
{
    QStringList keyList;
    for(const QString& key : mSchemaHelper.keys()) {
        if (mSchemaHelper[key]->required)
            keyList << key;
    }
    return keyList;
}

bool ConnectSchema::contains(const QString &key) const
{
    return (mSchemaHelper.contains(key));
}

Schema *ConnectSchema::getSchema(const QString &key) const
{
    qDebug() << mSchemaHelper.keys();
    if (contains(key))
        return mSchemaHelper[key];
    else
        return nullptr;
}

QList<SchemaType> ConnectSchema::getType(const QString &key) const
{
    if (contains(key)) {
        return mSchemaHelper[key]->types;
    } else {
        return QList<SchemaType>();
    }
}

QStringList ConnectSchema::getTypeAsStringList(const QString &key) const
{
    QStringList strlist;
    if (contains(key)) {
        foreach(SchemaType t, mSchemaHelper[key]->types) {
            int tt = (int)t;
            if ( tt==(int)SchemaValueType::Integer)
                strlist << "integer";
            else if ( tt==(int)SchemaValueType::Float)
                    strlist << "float";
            else if (tt==(int)SchemaValueType::String)
                     strlist << "string";
            else if (tt==(int)SchemaValueType::Boolean)
                     strlist << "boolean";
        }
    }
    return strlist;
}

QStringList ConnectSchema::getAllowedValueAsStringList(const QString &key) const
{
    QStringList strlist;
    if (contains(key)) {
        foreach(ValueWrapper vw, mSchemaHelper[key]->allowedValues) {
            int t = (int)vw.type;
            if ( t==(int)SchemaValueType::Integer)
                strlist << QString::number(vw.value.intval);
            else if ( t==(int)SchemaValueType::Float)
                    strlist << QString::number(vw.value.doubleval);
            else if (t==(int)SchemaValueType::String)
                     strlist << QString(vw.value.stringval);
            else if (t==(int)SchemaValueType::Boolean)
                     strlist << QString(vw.value.boolval);
        }
    }
    return strlist;
}

bool ConnectSchema::isRequired(const QString &key) const
{
    if (contains(key)) {
        return mSchemaHelper[key]->required;
    } else {
        return false;
    }
}

ValueWrapper ConnectSchema::getMin(const QString &key) const
{
    if (contains(key)) {
        return mSchemaHelper[key]->min;
    } else {
        return ValueWrapper();
    }
}

ValueWrapper ConnectSchema::getMax(const QString &key) const
{
    if (contains(key)) {
        return mSchemaHelper[key]->max;
    } else {
        return ValueWrapper();
    }
}

bool ConnectSchema::isSchemaDefined(const QString &key) const
{
    if (contains(key))
        return mSchemaHelper[key]->schemaDefined;

    return false;
}

Schema::Schema(int level_, YAML::Node schemaNode_, QList<SchemaType> type_, bool required_, QList<ValueWrapper> allowedValues_, ValueWrapper defaultValue_, ValueWrapper min_, ValueWrapper max_, bool schemaDefined_)
    : level(level_),
      schemaNode(schemaNode_),
      types(type_),
      required(required_),
      allowedValues(allowedValues_),
      defaultValue(defaultValue_),
      min(min_),
      max(max_),
      schemaDefined(schemaDefined_)
{ }


} // namespace connect
} // namespace studio
} // namespace gams