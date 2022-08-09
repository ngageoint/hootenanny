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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "PluginContext.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>

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
  Local<Context> context = ToLocal(&_context);
  context->AllowCodeGenerationFromStrings(true);

  // Create a template for the global object where we set the
  // built-in global functions.
  Local<Object> global = ToLocal(&_context)->Global();

  // Enter the created context for compiling and
  // running the hello world script.
  Context::Scope context_scope(ToLocal(&_context));

  Local<Object> hns = Object::New(current);
  global->Set(context, toV8("hoot"), hns);
  JsRegistrar::getInstance().initAll(hns);

  QStringList includes = ConfigOptions().getPluginContextIncludes();
  for (const auto& include : qAsConst(includes))
  {
    QString fullPath = ConfPath::search(include, "rules/lib");
    LOG_TRACE("Loading system include: " << fullPath);
    loadScript(fullPath);
  }
}

Local<Value> PluginContext::call(Local<Object> obj, QString name, QList<QVariant> args)
{
  Isolate* current = obj->GetIsolate();
  EscapableHandleScope handleScope(current);
  Local<Context> context = current->GetCurrentContext();
  if (obj->Has(context, toV8(name)).ToChecked() == false)
    throw InternalErrorException("Unable to find method in JS: " + name);

  Local<Value> value = obj->Get(context, toV8(name)).ToLocalChecked();
  if (value->IsFunction() == false)
    throw InternalErrorException("The specified object is not a function: " + name);

  Local<Function> func = Local<Function>::Cast(value);
  LOG_VART(args);
  vector<Local<Value>> jsArgs;

  for (const auto& arg : args)
    jsArgs.push_back(toValue(arg));

  MaybeLocal<Value> result = func->Call(context, obj, args.size(), jsArgs.data());

  if (result.IsEmpty())
    throw InternalErrorException("Function call '" + name + "' returned an error.");

  return handleScope.Escape(result.ToLocalChecked());
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

Local<Object> PluginContext::loadScript(QString filename, QString loadInto)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope escapableHandleScope(current);
  // Enter the created context for compiling and running the hello world script.
  Context::Scope context_scope(ToLocal(&_context));

  QFile fp(filename);
  LOG_TRACE("Loading script " << filename << "...");
  if (fp.open(QFile::ReadOnly) == false)
    throw HootException("Error opening script: " + filename);

  QString text = QString::fromUtf8(fp.readAll());
  Local<Object> result = loadText(text, loadInto, filename);

  return escapableHandleScope.Escape(result);
}

// This is a duplicate of loadScript. Will refactor later once the rest is working
Local<Object> PluginContext::loadText(QString text, QString loadInto, QString scriptName)
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope handleScope(current);
  Context::Scope context_scope(ToLocal(&_context));
  Local<Context> context = current->GetCurrentContext();

  MaybeLocal<Script> script;

  Local<Object> exports(Object::New(current));
  ToLocal(&_context)->Global()->Set(context, toV8("exports"), exports);

  TryCatch try_catch(current);

  // Compile the source code.
  ScriptOrigin origin(toV8(scriptName));
  script = Script::Compile(context, v8::String::NewFromUtf8(current, text.toUtf8().data()).ToLocalChecked(), &origin);

  if (script.IsEmpty())
    HootExceptionJs::throwAsHootException(try_catch);

  // Run the script to get the result.
  MaybeLocal<Value> result = script.ToLocalChecked()->Run(context);
  if (result.IsEmpty())
    HootExceptionJs::throwAsHootException(try_catch);

  HootExceptionJs::checkV8Exception(result.ToLocalChecked(), try_catch);

  if (loadInto != "")
    ToLocal(&_context)->Global()->Set(context, toV8(loadInto), exports);

  return handleScope.Escape(exports);
}


double PluginContext::toNumber(Local<Value> v, QString key, double defaultValue) const
{
  Isolate* current = v8::Isolate::GetCurrent();
  HandleScope handleScope(current);
  Local<Context> context = current->GetCurrentContext();
  if (v->IsObject() == false)
    throw IllegalArgumentException("Expected value to be an object.");

  Local<Object> obj = Local<Object>::Cast(v);
  Local<String> keyStr = v8::String::NewFromUtf8(current, key.toUtf8().data()).ToLocalChecked();
  double result = defaultValue;

  if (obj->Has(context, keyStr).ToChecked() == false)
  {
    if (defaultValue == UNSPECIFIED_DEFAULT)
      throw IllegalArgumentException("Expected object to have attribute " + key);
  }
  else
    result = obj->Get(context, keyStr).ToLocalChecked()->NumberValue(context).ToChecked();

  return result;
}

Local<Value> PluginContext::toValue(QVariant v) const
{
  Isolate* current = v8::Isolate::GetCurrent();
  EscapableHandleScope handleScope(current);
  Local<Value> result;
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
    result = toV8(v.toString());
  }

  return handleScope.Escape(result);
}

}
