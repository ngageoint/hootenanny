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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef STREAMUTILSJS_H
#define STREAMUTILSJS_H

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// v8
#include <hoot/js/HootJsStable.h>

namespace hoot
{

/**
 * @param qstr JSON string to parse
 * @param fileName Filename that is the source of this string. This is optional and is only used
 *  to report errors.
 * @return Parsed JSON value
 */
inline v8::Handle<v8::Value> fromJson(QString qstr, QString fileName="")
{
  v8::Isolate* current = v8::Isolate::GetCurrent();
  v8::EscapableHandleScope scope(current);
  v8::Handle<v8::Context> context = current->GetCurrentContext();
  v8::Handle<v8::Object> global = context->Global();

  QByteArray utf8 = qstr.toUtf8();
  v8::Handle<v8::Value> str =
    v8::String::NewFromUtf8(
      current, utf8.data(), v8::NewStringType::kNormal, utf8.length()).ToLocalChecked();

  v8::Handle<v8::Object> JSON = global->Get(v8::String::NewFromUtf8(current, "JSON"))->ToObject();
  v8::Handle<v8::Function> JSON_parse =
    v8::Handle<v8::Function>::Cast(JSON->Get(v8::String::NewFromUtf8(current, "parse")));

  v8::Handle<v8::Value> args[1];
  args[0] = str;

  v8::TryCatch tc;
  v8::Handle<v8::Value> result = JSON_parse->Call(JSON, 1, args);

  if (result.IsEmpty())
  {
    v8::Handle<v8::Message> msg = tc.Message();

    // See ReportException in http://v8.googlecode.com/svn/trunk/samples/shell.cc
    if (fileName.isEmpty())
    {
      fileName = toString(msg->GetScriptResourceName());
    }

    int lineNumber = msg->GetLineNumber();

    QString sourceLine = toString(msg->GetSourceLine());

    // Put a cool wavy line under the error
    int start = msg->GetStartColumn();
    int end = msg->GetEndColumn();
    QString blank(start,' ');
    QString wave(end - start,'^');

    throw HootException(QString("%1 (%2) \n%3\n%4").arg(fileName).arg(lineNumber).arg(sourceLine).
      arg(blank + wave));
  }

  return scope.Escape(result);
}

/**
 * Strangely, this class can sometimes cause objects to go wonky.
 */
template <class T>
QString toJson(const v8::Local<T> object)
{
  QString result;
  if (object.IsEmpty())
  {
    result = "";
  }
  else if (object->IsNull())
  {
    result = "null";
  }
  else if (object->IsUndefined())
  {
    result = "undefined";
  }
  else
  {
    v8::Isolate* current = v8::Isolate::GetCurrent();
    v8::HandleScope scope(current);

    v8::Local<v8::Context> context = current->GetCurrentContext();
    v8::Local<v8::Object> global = context->Global();

    v8::Local<v8::Object> JSON = global->Get(v8::String::NewFromUtf8(current, "JSON"))->ToObject();
    v8::Handle<v8::Function> JSON_stringify =
      v8::Handle<v8::Function>::Cast(JSON->Get(v8::String::NewFromUtf8(current, "stringify")));

    v8::Handle<v8::Value> args[1];
    args[0] = object;

    v8::Handle<v8::Value> resultValue = JSON_stringify->Call(JSON, 1, args);
    v8::Handle<v8::String> s = v8::Handle<v8::String>::Cast(resultValue);

    size_t utf8Length = s->Utf8Length() + 1;
    std::unique_ptr<char[]> buffer(new char[utf8Length]);
    s->WriteUtf8(buffer.get(), utf8Length);

    result = QString::fromUtf8(buffer.get());
  }

  return result;
}

std::ostream& operator<<(std::ostream& o, const v8::Handle<v8::Function>& f);

template <class T>
inline std::ostream& operator<<(std::ostream& o, const v8::Handle<T>& v)
{
  if (v.IsEmpty())
  {
    o << "<empty handle>";
  }
  else if (v->IsNull())
  {
    o << "<null>";
  }
  else if (v->IsUndefined())
  {
    o << "<undefined>";
  }
  else if (v->IsFunction())
  {
    v8::Handle<v8::Function> f = v8::Handle<v8::Function>::Cast(v);
    o << f;
  }
  else
  {
    o << toJson(v).toUtf8().data();
  }
  return o;
}

inline std::ostream& operator<<(std::ostream& o, const v8::Handle<v8::Function>& f)
{
  QString name = toJson(f->GetName());
  if (name != "\"\"")
  {
    o << name << "()";
  }
  else
  {
    o << "<function>";
  }
  return o;
}

inline std::ostream& operator<<(std::ostream& o, const v8::TryCatch& tc)
{
  v8::Handle<v8::Message> message = tc.Message();
  if (message.IsEmpty())
  {
    o << tc.Exception();
  }
  else
  {
    o << message->GetScriptResourceName() << ":" << message->GetLineNumber()
      << ": " << tc.Exception();
  }

  return o;
}

}

#endif // STREAMUTILSJS_H
