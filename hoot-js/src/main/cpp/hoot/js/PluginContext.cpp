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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#include "PluginContext.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// Qt
#include <QFile>

using namespace std;
using namespace v8;

namespace hoot
{

const double PluginContext::UNSPECIFIED_DEFAULT = -999999e9;

PluginContext::PluginContext()
{
  if (ConfigOptions().getHashSeedZero())
  {
    qSetGlobalQHashSeed(0);
    LOG_TRACE("Qt hash seed set to 0 for plugin");
  }

  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);

  // Create a new context.
  _context.Reset(current, Context::New(current));
  ToLocal(&_context)->AllowCodeGenerationFromStrings(true);

  // Create a template for the global object where we set the
  // built-in global functions.
  Handle<Object> global = ToLocal(&_context)->Global();

  // Enter the created context for compiling and
  // running the hello world script.
  Context::Scope context_scope(ToLocal(&_context));

  Handle<Object> hns = Object::New(current);
  global->Set(String::NewFromUtf8(current, "hoot"), hns);
  JsRegistrar::getInstance().initAll(hns);

  QStringList includes = ConfigOptions().getPluginContextIncludes();
  for (int i = 0; i < includes.size(); i++)
  {
    QString fullPath = ConfPath::search(includes[i], "rules/lib");
    LOG_TRACE("Loading system include: " << fullPath);
    loadScript(fullPath);
  }
}

PluginContext::~PluginContext()
{

}

Local<Value> PluginContext::call(Handle<Object> obj, QString name, QList<QVariant> args)
{
  Isolate* current = obj->GetIsolate();
  EscapableHandleScope handleScope(current);
  if (obj->Has(String::NewFromUtf8(current, name.toUtf8().data())) == false)
  {
    throw InternalErrorException("Unable to find method in JS: " + name);
  }
  Handle<Value> value = obj->Get(String::NewFromUtf8(current, name.toUtf8().data()));
  if (value->IsFunction() == false)
  {
    throw InternalErrorException("The specified object is not a function: " + name);
  }
  Handle<Function> func = Handle<Function>::Cast(value);
  vector<Handle<Value>> jsArgs(args.size());

  for (int i = 0; i < args.size(); i++)
  {
    jsArgs[i] = toValue(args[i]);
  }

  return handleScope.Escape(func->Call(obj, args.size(), jsArgs.data()));
}

Local<Value> PluginContext::eval(QString e)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope escapableHandleScope(current);
  return escapableHandleScope.Escape(call(ToLocal(&_context)->Global(), "eval", QVariantList() << e));
}

Local<Context> PluginContext::getContext(Isolate* isolate)
{
  EscapableHandleScope escapableHandleScope(isolate);
  return escapableHandleScope.Escape(ToLocal(&_context));
}


bool PluginContext::hasKey(Handle<Value> v, QString key)
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  if (v->IsObject() == false)
  {
    throw IllegalArgumentException("Expected value to be an object.");
  }
  Handle<Object> obj = Handle<Object>::Cast(v);

  Handle<String> keyStr = String::NewFromUtf8(current, key.toUtf8());
  return obj->Has(keyStr) == false;
}

Local<Object> PluginContext::loadScript(QString filename, QString loadInto)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope escapableHandleScope(current);
  // Enter the created context for compiling and
  // running the hello world script.
  Context::Scope context_scope(ToLocal(&_context));

  QFile fp(filename);
  LOG_TRACE("Loading script " << filename << "...");
  if (fp.open(QFile::ReadOnly) == false)
  {
    throw HootException("Error opening script: " + filename);
  }

  QString text = QString::fromUtf8(fp.readAll());

  Handle<Object> result = loadText(text, loadInto, filename);

  return escapableHandleScope.Escape(result);
}

// This is a duplicate of loadScript. Will refactor later once the rest is working
Local<Object> PluginContext::loadText(QString text, QString loadInto, QString scriptName)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope handleScope(current);
  Context::Scope context_scope(ToLocal(&_context));

  Local<Script> script;

  Local<Object> exports(Object::New(current));
  ToLocal(&_context)->Global()->Set(String::NewFromUtf8(current, "exports"), exports);

  TryCatch try_catch;

  // Compile the source code.
  ScriptOrigin origin(toV8(scriptName));
  script = Script::Compile(String::NewFromUtf8(current, text.toUtf8().data()), &origin);

  if (script.IsEmpty())
  {
    HootExceptionJs::throwAsHootException(try_catch);
  }

  // Run the script to get the result.
  HootExceptionJs::checkV8Exception(script->Run(), try_catch);

  if (loadInto != "")
  {
    ToLocal(&_context)->Global()->Set(String::NewFromUtf8(current, loadInto.toUtf8()), exports);
  }

  return handleScope.Escape(exports);
}


double PluginContext::toNumber(Handle<Value> v, QString key, double defaultValue)
{
  Isolate* current = v8::Isolate::GetCurrent();
  if (v->IsObject() == false)
  {
    throw IllegalArgumentException("Expected value to be an object.");
  }
  Handle<Object> obj = Handle<Object>::Cast(v);

  Handle<String> keyStr = String::NewFromUtf8(current, key.toUtf8());
  double result = defaultValue;
  if (obj->Has(keyStr) == false)
  {
    if (defaultValue == UNSPECIFIED_DEFAULT)
    {
      throw IllegalArgumentException("Expected object to have attribute " + key);
    }
  }
  else
  {
    result = obj->Get(keyStr)->NumberValue();
  }

  return result;
}

Local<Value> PluginContext::toValue(QVariant v)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope handleScope(current);
  Handle<Value> result;
  switch (v.type())
  {
  case QVariant::Int:
  case QVariant::UInt:
  case QVariant::LongLong:
  case QVariant::ULongLong:
  case QVariant::Double:
    result = Number::New(current, v.toDouble());
    break;
  default:
    result = String::NewFromUtf8(current, v.toString().toUtf8().data());
  }

  return handleScope.Escape(result);
}

}
