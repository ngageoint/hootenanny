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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef DATACONVERTJS_H
#define DATACONVERTJS_H

// hoot
#include <hoot/core/util/Units.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/HootJsStable.h>

namespace hoot
{

template<typename T>
T toCpp(v8::Local<v8::Value> v);

inline void toCpp(v8::Local<v8::Value> v, bool& o)
{
  if (v->IsTrue())
    o = true;
  else if (v->IsFalse())
    o = false;
  else
    throw IllegalArgumentException("Expected a boolean. Got: (" + toJson(v) + ")");
}

inline void toCpp(v8::Local<v8::Value> v, int& o)
{
  if (!v->IsInt32())
    throw IllegalArgumentException("Expected an integer. Got: (" + toJson(v) + ")");
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  o = v->Int32Value(context).ToChecked();
}

inline void toCpp(v8::Local<v8::Value> v, long& o)
{
  if (!v->IsInt32())
    throw IllegalArgumentException("Expected an integer. Got: (" + toJson(v) + ")");
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  o = v->Int32Value(context).ToChecked();
}

inline void toCpp(v8::Local<v8::Value> v, Meters& o)
{
  if (!v->IsNumber())
    throw IllegalArgumentException("Expected Meters (Number). Got: (" + toJson(v) + ")");
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  o = v->NumberValue(context).ToChecked();
}

template<typename T, typename U>
void toCpp(v8::Local<v8::Value> v, std::pair<T, U>& o)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  if (!v->IsArray())
    throw IllegalArgumentException("While converting a pair, expected an array. Got: (" + toJson(v) + ")");
  v8::Local<v8::Array> arr = v8::Local<v8::Array>::Cast(v);
  if (arr->Length() != 2)
    throw IllegalArgumentException("Expected an array of length 2, but got (" + toJson(v) + ")");
  toCpp(arr->Get(context, 0).ToLocalChecked(), o.first);
  toCpp(arr->Get(context, 1).ToLocalChecked(), o.second);
}

/**
 * The string conversion routine is a bit arbitrary. We support converting primitive types into
 * strings (e.g. number, string, bool), but not object types. This is simply because strings
 * may be automagically converted to primitive types. E.g. '1' may be turned into a number.
 */
inline void toCpp(v8::Local<v8::Value> v, QString& s)
{
  if (v.IsEmpty() || v->IsUndefined() || v->IsNull())
    throw IllegalArgumentException("Expected a string. Got an empty value.");
  if (v->IsString() || v->IsNumber() || v->IsBoolean())
  {
    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::HandleScope scope(current);
    v8::Local<v8::Context> context = current->GetCurrentContext();
    v8::String::Utf8Value param(current, v->ToString(context).ToLocalChecked());
    s = QString::fromUtf8(*param);
  }
  else
    throw IllegalArgumentException("Expected a string. Got: (" + toJson(v) + ")");
}

inline void toCpp(v8::Local<v8::Value> v, QStringList& o)
{
  if (!v->IsArray())
    throw IllegalArgumentException("Expected an array. Got: (" + toJson(v) + ")");
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(v);

  o.reserve(arr->Length());
  for (uint32_t i = 0; i < arr->Length(); i++)
    o.append(toCpp<QString>(arr->Get(context, i).ToLocalChecked()));
}

inline void toCpp(v8::Local<v8::Value> v, QVariantList& l)
{
  if (v.IsEmpty() || v->IsArray() == false)
    throw IllegalArgumentException("Expected to get an array. Got: (" + toJson(v) + ")");
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(v);

  l.clear();
  l.reserve(arr->Length());
  for (uint32_t i = 0; i < arr->Length(); i++)
    l.append(toCpp<QVariant>(arr->Get(context, i).ToLocalChecked()));
}

inline void toCpp(v8::Local<v8::Value> v, QVariantMap& m)
{
  if (v.IsEmpty() || v->IsObject() == false)
    throw IllegalArgumentException("Expected to get an object. Got: (" + toJson(v) + ")");
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();

  v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(v);

  v8::Local<v8::Array> arr = obj->GetPropertyNames(context).ToLocalChecked();
  for (uint32_t i = 0; i < arr->Length(); i++)
  {
    QString key = toCpp<QString>(arr->Get(context, i).ToLocalChecked());
    QVariant val = toCpp<QVariant>(obj->Get(context, arr->Get(context, i).ToLocalChecked()).ToLocalChecked());
    m.insert(key, val);
  }
}

inline void toCpp(v8::Local<v8::Value> v, QVariant& qv)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  if (v.IsEmpty() || v->IsUndefined() || v->IsNull())
    qv = QVariant();
  else if (v->IsString())
    qv = toCpp<QString>(v);
  else if (v->IsInt32())
    qv.setValue(v->Int32Value(context).ToChecked());
  else if (v->IsNumber())
    qv.setValue(v->NumberValue(context).ToChecked());
  else if (v->IsArray())
    qv = toCpp<QVariantList>(v);
  else if (v->IsObject())
  {
    QVariantMap m;
    toCpp(v, m);
    qv = m;
  }
  else
    throw IllegalArgumentException("Received unexpected data type: " + toJson(v));
}

template<typename T>
void toCpp(v8::Local<v8::Value> v, std::vector<T>& o)
{
  if (!v->IsArray())
    throw IllegalArgumentException("Expected an array. Got: (" + toJson(v) + ")");
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(v);

  o.resize(arr->Length());
  for (uint32_t i = 0; i < arr->Length(); i++)
  {
    T t;
    toCpp(arr->Get(context, i).ToLocalChecked(), t);
    o[i] = t;
  }
}

/**
 * std::set is converted to a JavaScript Array. Using objects coerces all the keys into strings.
 */
template<typename T>
void toCpp(v8::Local<v8::Value> v, std::set<T>& o)
{
  if (!v->IsArray())
    throw IllegalArgumentException("Expected an array. Got: (" + toJson(v) + ")");
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::HandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  v8::Handle<v8::Array> arr = v8::Handle<v8::Array>::Cast(v);
  for (uint32_t i = 0; i < arr->Length(); i++)
  {
    T t;
    toCpp(arr->Get(context, i).ToLocalChecked(), t);
    o.insert(t);
  }
}

template<typename T>
inline T toCpp(v8::Local<v8::Value> v)
{
  T t;
  toCpp(v, t);
  return t;
}

inline v8::Local<v8::Value> toV8(bool v)
{
  v8::EscapableHandleScope scope(v8::Isolate::GetCurrent());
  return scope.Escape(v8::Boolean::New(v8::Isolate::GetCurrent(), v));
}

inline v8::Local<v8::Value> toV8(int i)
{
  v8::EscapableHandleScope scope(v8::Isolate::GetCurrent());
  return scope.Escape(v8::Integer::New(v8::Isolate::GetCurrent(), i));
}

inline v8::Local<v8::Value> toV8(double v)
{
  v8::EscapableHandleScope scope(v8::Isolate::GetCurrent());
  return scope.Escape(v8::Number::New(v8::Isolate::GetCurrent(),v));
}

inline v8::Local<v8::Value> toV8(const std::string& s)
{
  v8::EscapableHandleScope scope(v8::Isolate::GetCurrent());
  return scope.Escape(v8::String::NewFromUtf8(
      v8::Isolate::GetCurrent(), s.data(), v8::NewStringType::kNormal, static_cast<int>(s.length())).ToLocalChecked());
}

template<typename T, typename U>
v8::Local<v8::Value> toV8(const std::pair<T, U>& p)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::EscapableHandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  v8::Handle<v8::Array> result = v8::Array::New(v8::Isolate::GetCurrent(), 2);
  result->Set(context, 0, toV8(p.first));
  result->Set(context, 1, toV8(p.second));
  return scope.Escape(result);
}

template<typename T>
v8::Local<v8::Value> toV8(const std::vector<T>& v)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::EscapableHandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  v8::Handle<v8::Array> result = v8::Array::New(v8::Isolate::GetCurrent(), static_cast<int>(v.size()));
  for (uint32_t i = 0; i < v.size(); i++)
    result->Set(context, i, toV8(v[i]));
  return scope.Escape(result);
}

/**
 * std::set is converted to a JavaScript Array. Using objects coerces all the keys into strings.
 */
template<typename T>
v8::Local<v8::Value> toV8(const std::set<T>& v)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::EscapableHandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  v8::Handle<v8::Array> result = v8::Array::New(v8::Isolate::GetCurrent(), static_cast<int>(v.size()));
  uint32_t i = 0;
  for (typename std::set<T>::const_iterator it = v.begin(); it != v.end(); ++it)
    result->Set(context, i++, toV8(*it));
  return scope.Escape(result);
}

inline v8::Local<v8::Value> toV8(const char* s)
{
  v8::EscapableHandleScope scope(v8::Isolate::GetCurrent());
  return scope.Escape(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), s).ToLocalChecked());
}

inline v8::Local<v8::Value> toV8(const QString& s)
{
  v8::EscapableHandleScope scope(v8::Isolate::GetCurrent());
  QByteArray utf8 = s.toUtf8();
  return scope.Escape(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), utf8.data(), v8::NewStringType::kNormal, utf8.length()).ToLocalChecked());
}

inline v8::Local<v8::Value> toV8(const QStringList& v)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::EscapableHandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  v8::Handle<v8::Array> result = v8::Array::New(v8::Isolate::GetCurrent(), v.size());
  for (int i = 0; i < v.size(); i++)
    result->Set(context, i, toV8(v[i]));
  return scope.Escape(result);
}

inline v8::Local<v8::Value> toV8(const QVariant& v)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::EscapableHandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  switch (v.type())
  {
  case QVariant::Invalid:
    return scope.Escape(v8::Undefined(current));
  case QVariant::Bool:
    return scope.Escape(v8::Boolean::New(current, v.toBool()));
  case QVariant::Double:
    return scope.Escape(v8::Number::New(current, v.toDouble()));
  case QVariant::Int:
    return scope.Escape(v8::Integer::New(current, v.toInt()));
  case QVariant::String:
    return scope.Escape(toV8(v.toString()));
  case QVariant::StringList:
    return scope.Escape(toV8(v.toStringList()));
  case QVariant::List:
  {
    QVariantList l = v.toList();
    v8::Local<v8::Array> result = v8::Array::New(current, l.size());
    int i = 0;
    for (const auto& value : qAsConst(l))
      result->Set(context, i++, toV8(value));
    return scope.Escape(result);
  }
  case QVariant::Hash:
  {
    v8::Local<v8::Object> result = v8::Object::New(current);
    QVariantHash m = v.toHash();
    for (auto i = m.cbegin(); i != m.cend(); i++)
      result->Set(context, toV8(i.key()),toV8(i.value()));
    return scope.Escape(result);
  }
  case QVariant::Map:
  {
    v8::Local<v8::Object> result = v8::Object::New(current);
    QVariantMap m = v.toMap();
    for (auto i = m.cbegin(); i != m.cend(); i++)
      result->Set(context, toV8(i.key()),toV8(i.value()));
    return scope.Escape(result);
  }
  default:
    throw IllegalArgumentException("Received unexpected data type: " + v.toString() + "(" +
                                   v.typeName() + ")");
  }
}

template<typename T, typename U>
inline v8::Local<v8::Value> toV8(const QHash<T, U>& m)
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::EscapableHandleScope scope(current);
  v8::Local<v8::Context> context = current->GetCurrentContext();
  v8::Handle<v8::Object> result = v8::Object::New(v8::Isolate::GetCurrent());
  for (auto i = m.begin(); i != m.end(); ++i)
    result->Set(context, toV8(i.key()),toV8(i.value()));
  return scope.Escape(result);
}

}

#endif // DATACONVERTJS_H
