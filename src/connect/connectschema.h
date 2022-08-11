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
#ifndef CONNECTSCHEMA_H
#define CONNECTSCHEMA_H

#include <QVector>
#include <QMap>
#include <QList>
#include <QMetaEnum>
#include <QMetaType>
#include "connectagent.h"

namespace gams {
namespace studio {
namespace connect {

Q_NAMESPACE

enum class SchemaType {
    String,
    Integer,
    Float,
    Boolean,
    List,
    Dict,
    Undefined = -1
};
Q_ENUM_NS(SchemaType)

enum class SchemaValueType {
    Integer,
    Float,
    String,
    Boolean,
    NoValue,
    Undefined = -1
};
Q_ENUM_NS(SchemaValueType)

union Value  {
    bool    boolval;
    int     intval;
    double  doubleval;
    char*   stringval;

    Value() { memset(this, 0, sizeof(Value));  }
    Value(bool val) : boolval(val)  { }
    Value(int val)  : intval(val)   { }
    Value(double val) : doubleval(val) { }
    Value(const char* val) : stringval(val != NULL ? strdup(val) : NULL)  { }
    Value(const std::string& val) : stringval(strdup(val.c_str()))       { }

    Value& operator=(bool val)  {
       boolval = val;
       return *this;
    }
    Value& operator=(int val)   {
       intval = val;
       return *this;
    }
    Value& operator=(double val) {
       doubleval = val;
       return *this;
    }
    Value& operator=(const char* val) {
       stringval = val != NULL ? strdup(val) : NULL;
        return *this;
    }
    Value& operator=(const std::string& val) {
       stringval = strdup(val.c_str());
       return *this;
    }

//    bool operator<(int val) const     { return intval < val;     }
//    bool operator>(int val) const     { return intval > val;     }
//    bool operator<(double val) const  { return doubleval < val;  }
//    bool operator>(double val) const  { return doubleval > val;  }
    bool operator==(bool val) const   { return boolval == val;   }
    bool operator==(int val) const    { return intval == val;    }
    bool operator==(double val) const { return doubleval == val; }
    bool operator==(const char* val) const {
        if( stringval == val )
           return true;
        if( stringval == NULL || val == NULL )
           return false;
        return strcmp(stringval, val) == 0;
    }

    bool operator==(const std::string& val) const {
        return operator==(val.c_str());
    }

    template <typename T>
    bool operator!=(T val) const {
       return !operator==(val);
    }
};

struct ValueWrapper {
    SchemaValueType   type;
    union Value value;
    ValueWrapper()                    : type(SchemaValueType::NoValue) {  }
    ValueWrapper(int intval_)         : type(SchemaValueType::Integer), value(intval_)    { }
    ValueWrapper(double doubleval_)   : type(SchemaValueType::Float),   value(doubleval_) { }
    ValueWrapper(bool boolval_)       : type(SchemaValueType::Boolean), value(boolval_) { }
    ValueWrapper(std::string strval_) : type(SchemaValueType::String),  value(strval_) { }
};


class Schema {
public:
    int           level;
    QList<SchemaType>   types;
    bool          required;
    QList<ValueWrapper>  allowedValues;
    ValueWrapper         defaultValue;
    ValueWrapper         min;
    ValueWrapper         max;
    bool                 schemaDefined;

    Schema(
        int                 level_,
        QList<SchemaType>         type_,
        bool                required_,
        QList<ValueWrapper> allowedValues_
    ) : level(level_),
        types(type_),
        required(required_),
        allowedValues(allowedValues_)
      { }

    Schema(
        int           level_,
        QList<SchemaType>   type_,
        bool          required_,
        QList<ValueWrapper>  allowedValues_,
        ValueWrapper         defaultValue_,
        ValueWrapper         min_,
        ValueWrapper         max_,
        bool                 schemaDefined_=false
    )
    : level(level_),
      types(type_),
      required(required_),
      allowedValues(allowedValues_),
      defaultValue(defaultValue_),
      min(min_),
      max(max_),
      schemaDefined(schemaDefined_)
    { }

    bool hasType(SchemaType tt) {
       for (const SchemaType t : types) {
          if (t==tt) return true;
       }
       return false;
    }

};


class ConnectSchema : public ConnectAgent
{
private:
    QMap<QString, Schema*> mSchemaHelper;
    QStringList            mOrderedKeyList;

    friend class Connect;

    void createSchemaHelper(QString& key, const YAML::Node& node, int level);

public:
    ConnectSchema(const QString& inputFileName);
    ~ConnectSchema();

    void loadFromFile(const QString& inputFileName);
    void loadFromString(const QString& input);

    QStringList getlKeyList() const;
    QStringList getFirstLevelKeyList() const;
    QStringList getNextLevelKeyList(const QString& key) const;
    QStringList getAllRequiredKeyList() const;
    bool contains(const QString& key) const;

    Schema* getSchema(const QString& key) const;

    QList<SchemaType> getType(const QString& key) const;
    QStringList getTypeAsStringList(const QString& key) const;

    QStringList getAllowedValueAsStringList(const QString& key) const;
    bool isRequired(const QString& key) const;
    ValueWrapper getMin(const QString& key) const;
    ValueWrapper getMax(const QString& key) const;
    bool isSchemaDefined(const QString& key) const;

    static inline SchemaType getTypeFromValue(QString& value) {
        if (value.compare("integer") == 0) {
            return SchemaType::Integer;
        } else if (value.compare("string") == 0) {
                  return SchemaType::String;
        } else if (value.compare("float") == 0) {
                  return SchemaType::String;
        } else if (value.compare("boolean") == 0) {
                   return SchemaType::Boolean;
        } else if (value.compare("float") == 0) {
                   return SchemaType::Float;
        } else if (value.compare("dict") == 0) {
                   return SchemaType::Dict;
        } else if (value.compare("list") == 0) {
                  return SchemaType::List;
        }
        return SchemaType::Undefined;
    }

    static inline const char* typeToString(SchemaType t) {
        const std::map<SchemaType, const char*> typeStrings {
            { SchemaType::Integer, "integer" },
            { SchemaType::String,  "string" },
            { SchemaType::Float,   "float" },
            { SchemaType::Boolean, "boolean" },
            { SchemaType::Dict,    "dict" },
            { SchemaType::List,    "list" },
            { SchemaType::Undefined, "undefined" }
        };
        auto   it  = typeStrings.find(t);
        return (it == typeStrings.end() ? "undefined" : it->second);
    }
};

} // namespace connect
} // namespace studio
} // namespace gams

#endif // CONNECTSCHEMA_H
