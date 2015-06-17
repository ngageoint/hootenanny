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
 * @copyright Copyright (C) 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef STREAMUTILSJS_H
#define STREAMUTILSJS_H

// hoot
#include <hoot/core/util/Log.h>

// v8
#include <v8.h>

namespace hoot
{
using namespace v8;

inline Handle<Value> fromJson(QString qstr)
{
  HandleScope scope;
  Handle<Context> context = Context::GetCurrent();
  Handle<Object> global = context->Global();

  QByteArray utf8 = qstr.toUtf8();
  Handle<Value> str = v8::String::New(utf8.data(), utf8.length());

  Handle<Object> JSON = global->Get(String::New("JSON"))->ToObject();
  Handle<Function> JSON_parse = Handle<Function>::Cast(JSON->Get(String::New("parse")));

  Handle<Value> args[1];
  args[0] = str;
  return scope.Close(JSON_parse->Call(JSON, 1, args));
}

/**
 * @todo strangely this class can sometimes cause objects to go wonky.
 */
template <class T>
QString toJson(const Handle<T> object)
{
  HandleScope scope;

  Local<Context> context = Context::GetCurrent();
  Local<Object> global = context->Global();

  Local<Object> JSON = global->Get(String::New("JSON"))->ToObject();
  Handle<Function> JSON_stringify = Handle<Function>::Cast(JSON->Get(String::New("stringify")));

  Handle<Value> args[1];
  args[0] = object;
  Handle<Value> result = JSON_stringify->Call(JSON, 1, args);
  Handle<String> s = Handle<String>::Cast(result);

  size_t utf8Length = s->Utf8Length() + 1;
  std::auto_ptr<char> buffer(new char[utf8Length]);
  s->WriteUtf8(buffer.get(), utf8Length);

  return QString::fromUtf8(buffer.get());
}

std::ostream& operator<<(std::ostream& o, const v8::Handle<Function>& f);

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
    Handle<Function> f = Handle<Function>::Cast(v);
    o << f;
  }
  else
  {
    o << toJson(v).toUtf8().data();
  }
  return o;
}

inline std::ostream& operator<<(std::ostream& o, const v8::Handle<Function>& f)
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
