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
#include "PluginContext.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/HootException.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/HootExceptionJs.h>

// Qt
#include <QFile>

namespace hoot
{

const double PluginContext::UNSPECIFIED_DEFAULT = -999999e9;

PluginContext::PluginContext()
{
  HandleScope handleScope;

  // Create a new context.
  _context = Context::New();
  _context->AllowCodeGenerationFromStrings(true);

  // Create a template for the global object where we set the
  // built-in global functions.
  Handle<Object> global = _context->Global();

  // Enter the created context for compiling and
  // running the hello world script.
  Context::Scope context_scope(_context);

  Handle<v8::Object> hns = Object::New();
  global->Set(String::New("hoot"), hns);
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
  HandleScope handleScope;
  if (obj->Has(String::NewSymbol(name.toUtf8().data())) == false)
  {
    throw InternalErrorException("Unable to find method in JS: " + name);
  }
  Handle<v8::Value> value = obj->Get(String::NewSymbol(name.toUtf8().data()));
  if (value->IsFunction() == false)
  {
    throw InternalErrorException("The specified object is not a function: " + name);
  }
  Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);
  vector< Handle<Value> > jsArgs(args.size());

  for (int i = 0; i < args.size(); i++)
  {
    jsArgs[i] = toValue(args[i]);
  }

  return handleScope.Close(func->Call(obj, args.size(), jsArgs.data()));
}

Local<Value> PluginContext::eval(QString e)
{
  return call(getContext()->Global(), "eval", QVariantList() << e);
}

bool PluginContext::hasKey(Handle<Value> v, QString key)
{
  if (v->IsObject() == false)
  {
    throw IllegalArgumentException("Expected value to be an object.");
  }
  Handle<Object> obj = Handle<Object>::Cast(v);

  Handle<String> keyStr = String::New(key.toUtf8());
  return obj->Has(keyStr) == false;
}

Local<Object> PluginContext::loadScript(QString filename, QString loadInto)
{
  HandleScope handleScope;
  // Enter the created context for compiling and
  // running the hello world script.
  Context::Scope context_scope(_context);

  QFile fp(filename);
  if (fp.open(QFile::ReadOnly) == false)
  {
    throw HootException("Error opening script: " + filename);
  }

  QString text = QString::fromUtf8(fp.readAll());

  Handle<Object> result = loadText(text, loadInto, filename);

  return handleScope.Close(result);
}

// This is a duplicate of loadScript. Will refactor later once the rest is working
Local<Object> PluginContext::loadText(QString text, QString loadInto, QString scriptName)
{
  HandleScope handleScope;
  Context::Scope context_scope(_context);

  Handle<Script> script;

  Handle<Object> exports(Object::New());
  _context->Global()->Set(String::New("exports"), exports);

  TryCatch try_catch;

  // Compile the source code.
  v8::ScriptOrigin origin(toV8(scriptName));
  script = Script::Compile(String::New(text.toUtf8().data()), &origin);

  if (script.IsEmpty())
  {
    HootExceptionJs::throwAsHootException(try_catch);
  }

  // Run the script to get the result.
  HootExceptionJs::checkV8Exception(script->Run(), try_catch);

  if (loadInto != "")
  {
    _context->Global()->Set(String::New(loadInto.toUtf8()), exports);
  }

  return handleScope.Close(exports);
}


double PluginContext::toNumber(Handle<Value> v, QString key, double defaultValue)
{
  if (v->IsObject() == false)
  {
    throw IllegalArgumentException("Expected value to be an object.");
  }
  Handle<Object> obj = Handle<Object>::Cast(v);

  Handle<String> keyStr = String::New(key.toUtf8());
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
  HandleScope handleScope;
  Handle<Value> result;
  switch (v.type())
  {
  case QVariant::Int:
  case QVariant::UInt:
  case QVariant::LongLong:
  case QVariant::ULongLong:
  case QVariant::Double:
    result = Number::New(v.toDouble());
    break;
  default:
    result = String::New(v.toString().toUtf8().data());
  }

  return handleScope.Close(result);
}

}
