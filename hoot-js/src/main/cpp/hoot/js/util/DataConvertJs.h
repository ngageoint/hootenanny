/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef DATACONVERTJS_H
#define DATACONVERTJS_H

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Units.h>

// Qt
#include <QHash>
#include <QStringList>

// v8
#include <v8.h>

#include "StreamUtilsJs.h"

namespace hoot
{

template<typename T>
T toCpp(v8::Handle<v8::Value> v);

inline void toCpp(v8::Handle<v8::Value> v, bool& o)
{
  if (v->IsTrue())
  {
    o = true;
  }
  else if (v->IsFalse())
  {
    o = false;
  }
  else
  {
    throw IllegalArgumentException("Expected a boolean. Got: (" + toJson(v) + ")");
  }
}

inline void toCpp(v8::Handle<v8::Value> v, int& o)
{
  if (!v->IsInt32())
  {
    throw IllegalArgumentException("Expected an integer. Got: (" + toJson(v) + ")");
  }
  o = v->Int32Value();
}

inline void toCpp(v8::Handle<v8::Value> v, long& o)
{
  if (!v->IsInt32())
  {
    throw IllegalArgumentException("Expected an integer. Got: (" + toJson(v) + ")");
  }
  o = v->Int32Value();
}

inline void toCpp(v8::Handle<v8::Value> v, Meters& o)
{
  if (!v->IsNumber())
  {
    throw IllegalArgumentException("Expected Meters (Number). Got: (" + toJson(v) + ")");
  }
  o = v->NumberValue();
}

template<typename T, typename U>
void toCpp(v8::Handle<v8::Value> v, pair<T, U>& o)
{
  if (!v->IsArray())
  {
    throw IllegalArgumentException("While converting a pair, expected an array. Got: (" +
                                   toJson(v) + ")");
  }
  v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(v);
  if (arr->Length() != 2)
  {
    throw IllegalArgumentException("Expected an array of length 2, but got (" + toJson(v) + ")");
  }

  toCpp(arr->Get(0), o.first);
  toCpp(arr->Get(1), o.second);
}

/**
 * The string conversion routine is a bit arbitrary. We support converting primitive types into
 * strings (e.g. number, string, bool), but not object types. This is simply because strings
 * may be automagically converted to primitive types. E.g. '1' may be turned into a number.
 */
inline void toCpp(v8::Handle<v8::Value> v, QString& s)
{
  if (v.IsEmpty())
  {
    throw IllegalArgumentException("Expected a string. Got an empty value.");
  }
  Handle<String> str;
  if (v->IsString() || v->IsNumber() || v->IsBoolean())
  {
    str = v->ToString();
  }
  else
  {
    throw IllegalArgumentException("Expected a string. Got: (" + toJson(v) + ")");
  }

  v8::String::Utf8Value param(str);
  s = QString::fromUtf8(*param);
}

inline void toCpp(v8::Handle<v8::Value> v, QStringList& o)
{
  if (!v->IsArray())
  {
    throw IllegalArgumentException("Expected an array. Got: (" + toJson(v) + ")");
  }
  v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(v);

  o.reserve(arr->Length());
  for (uint32_t i = 0; i < arr->Length(); i++)
  {
    o.append(toCpp<QString>(arr->Get(i)));
  }
}

inline void toCpp(v8::Handle<v8::Value> v, QVariantList& l)
{
  if (v.IsEmpty() || v->IsArray() == false)
  {
    throw IllegalArgumentException("Expected to get an array. Got: (" + toJson(v) + ")");
  }
  v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(v);

  l.clear();
  l.reserve(arr->Length());
  for (uint32_t i = 0; i < arr->Length(); i++)
  {
    l.append(toCpp<QVariant>(arr->Get(i)));
  }
}

inline void toCpp(v8::Handle<v8::Value> v, QVariantMap& m)
{
  if (v.IsEmpty() || v->IsObject() == false)
  {
    throw IllegalArgumentException("Expected to get an object. Got: (" + toJson(v) + ")");
  }
  v8::Handle<v8::Object> obj = v8::Handle<v8::Object>::Cast(v);

  v8::Local<v8::Array> arr = obj->GetPropertyNames();
  for (uint32_t i = 0; i < arr->Length(); i++)
  {
    QString k = toCpp<QString>(arr->Get(i));
    QVariant v = toCpp<QVariant>(obj->Get(arr->Get(i)));

    m.insert(k, v);
  }
}

inline void toCpp(v8::Handle<v8::Value> v, QVariant& qv)
{
  if (v.IsEmpty() || v->IsUndefined() || v->IsNull())
  {
    qv = QVariant();
  }
  else if (v->IsString())
  {
    qv = toCpp<QString>(v);
  }
  else if (v->IsInt32())
  {
    // Changed this since OGR is expecting Int and this makes a Long
    // It throws an error in OgrWriter.cpp:_addFeature
//    qv.setValue(v->IntegerValue());
    qv.setValue(v->Int32Value());
  }
  else if (v->IsNumber())
  {
    qv.setValue(v->NumberValue());
  }
  else if (v->IsArray())
  {
    qv = toCpp<QVariantList>(v);
  }
  else if (v->IsObject())
  {
    QVariantMap m;
    toCpp(v, m);
    qv = m;
  }
  else
  {
    throw IllegalArgumentException("Received unexpected data type: " + toJson(v));
  }
}

template<typename T>
void toCpp(v8::Handle<v8::Value> v, vector<T>& o)
{
  if (!v->IsArray())
  {
    throw IllegalArgumentException("Expected an array. Got: (" + toJson(v) + ")");
  }
  v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(v);

  o.resize(arr->Length());
  for (uint32_t i = 0; i < arr->Length(); i++)
  {
    T t;
    toCpp(arr->Get(i), t);
    o[i] = t;
  }
}

/**
 * std::set is converted to a JavaScript Array. Using objects coerces all the keys into strings.
 */
template<typename T>
void toCpp(v8::Handle<v8::Value> v, set<T>& o)
{
  if (!v->IsArray())
  {
    throw IllegalArgumentException("Expected an array. Got: (" + toJson(v) + ")");
  }
  v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(v);
  for (uint32_t i = 0; i < arr->Length(); i++)
  {
    T t;
    toCpp(arr->Get(i), t);
    o.insert(t);
  }
}

template<typename T>
inline T toCpp(v8::Handle<v8::Value> v)
{
  T t;
  toCpp(v, t);
  return t;
}

inline v8::Handle<v8::Value> toV8(bool v)
{
  return v8::Boolean::New(v);
}

inline v8::Handle<v8::Value> toV8(int i)
{
  return v8::Integer::New(i);
}

inline v8::Handle<v8::Value> toV8(double v)
{
  return v8::Number::New(v);
}

inline v8::Handle<v8::Value> toV8(const std::string& s)
{
  return v8::String::New(s.data(), s.length());
}

template<typename T, typename U>
v8::Handle<v8::Value> toV8(const std::pair<T, U>& p)
{
  v8::Handle<v8::Array> result = v8::Array::New(2);
  result->Set(0, toV8(p.first));
  result->Set(1, toV8(p.second));
  return result;
}

template<typename T>
v8::Handle<v8::Value> toV8(const std::vector<T>& v)
{
  v8::Handle<v8::Array> result = v8::Array::New(v.size());

  for (uint32_t i = 0; i < v.size(); i++)
  {
    result->Set(i, toV8(v[i]));
  }

  return result;
}

/**
 * std::set is converted to a JavaScript Array. Using objects coerces all the keys into strings.
 */
template<typename T>
v8::Handle<v8::Value> toV8(const std::set<T>& v)
{
  v8::Handle<v8::Array> result = v8::Array::New(v.size());

  uint32_t i = 0;
  for (typename std::set<T>::const_iterator it = v.begin(); it != v.end(); ++it)
  {
    result->Set(i++, toV8(*it));
  }

  return result;
}

inline v8::Handle<v8::Value> toV8(const char* s)
{
  return v8::String::New(s);
}

inline v8::Handle<v8::Value> toV8(const QString& s)
{
  QByteArray utf8 = s.toUtf8();
  return v8::String::New(utf8.data(), utf8.length());
}

inline v8::Handle<v8::Value> toV8(const QStringList& v)
{
  v8::Handle<v8::Array> result = v8::Array::New(v.size());
  for (int i = 0; i < v.size(); i++)
  {
    result->Set(i, toV8(v[i]));
  }

  return result;
}

inline v8::Handle<v8::Value> toV8(const QVariant& v)
{
  switch (v.type())
  {
  case QVariant::Invalid:
    return Undefined();
  case QVariant::Bool:
    return v8::Boolean::New(v.toBool());
  case QVariant::Double:
    return v8::Number::New(v.toDouble());
  case QVariant::Int:
    return v8::Integer::New(v.toInt());
  case QVariant::String:
    return toV8(v.toString());
  case QVariant::StringList:
    return toV8(v.toStringList());
  case QVariant::List:
    {
      QVariantList l = v.toList();
      v8::Handle<v8::Array> result = v8::Array::New(l.size());
      for (int i = 0; i < l.size(); i++)
      {
        result->Set(i, toV8(l[i]));
      }
      return result;
    }
  case QVariant::Hash:
  {
    v8::Handle<v8::Object> result = v8::Object::New();
    QVariantHash m = v.toHash();

    for (QVariantHash::const_iterator i = m.begin(); i != m.end(); i++)
    {
      result->Set(toV8(i.key()),toV8(i.value()),None);
    }

    return result;
  }
  case QVariant::Map:
  {
    v8::Handle<v8::Object> result = v8::Object::New();
    QVariantMap m = v.toMap();

    for (QVariantMap::const_iterator i = m.begin(); i != m.end(); i++)
    {
      result->Set(toV8(i.key()),toV8(i.value()),None);
    }

    return result;
  }
  default:
    throw IllegalArgumentException("Received unexpected data type: " + v.toString() + "(" +
                                   v.typeName() + ")");
  }
}

template<typename T, typename U>
inline v8::Handle<v8::Value> toV8(const QHash<T, U>& m)
{
  v8::Handle<v8::Object> result = v8::Object::New();

  typename QHash<T, U>::const_iterator i;
  for (i = m.begin(); i != m.end(); i++)
  {
    result->Set(toV8(i.key()),toV8(i.value()),None);
  }

  return result;
}

}

#endif // DATACONVERTJS_H
