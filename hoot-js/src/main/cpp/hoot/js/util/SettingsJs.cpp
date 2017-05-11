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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SettingsJs.h"

// hoot
#include <hoot/core/util/Settings.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/DataConvertJs.h>
#include <hoot/js/util/StringUtilsJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(SettingsJs)

SettingsJs::SettingsJs()
{
}

SettingsJs::~SettingsJs() {}

void SettingsJs::Init(Handle<Object> exports)
{
  Handle<Object> settings = Object::New();
  exports->Set(String::NewSymbol("Settings"), settings);
  exports->Set(String::NewSymbol("get"), FunctionTemplate::New(get)->GetFunction());
  settings->Set(String::NewSymbol("get"), FunctionTemplate::New(get)->GetFunction());
  exports->Set(String::NewSymbol("getAll"), FunctionTemplate::New(getAll)->GetFunction());
  settings->Set(String::NewSymbol("getAll"), FunctionTemplate::New(getAll)->GetFunction());
  exports->Set(String::NewSymbol("getValue"), FunctionTemplate::New(getValue)->GetFunction());
  settings->Set(String::NewSymbol("getValue"), FunctionTemplate::New(getValue)->GetFunction());
  exports->Set(String::NewSymbol("set"), FunctionTemplate::New(set)->GetFunction());
  settings->Set(String::NewSymbol("set"), FunctionTemplate::New(set)->GetFunction());
  exports->Set(String::NewSymbol("loadJson"), FunctionTemplate::New(loadJson)->GetFunction());
  settings->Set(String::NewSymbol("loadJson"), FunctionTemplate::New(loadJson)->GetFunction());
}

Handle<Value> SettingsJs::get(const Arguments& args) {
  HandleScope scope;

  Settings* settings = &conf();

  QString key = str(args[0]->ToString());
  if (settings->hasKey(key))
  {
    QString value = settings->getString(key);
    return scope.Close(String::New(value.toUtf8().data()));
  }
  else
  {
    return scope.Close(Undefined());
  }
}

Handle<Value> SettingsJs::getAll(const Arguments&) {
  HandleScope scope;

  Settings* settings = &conf();

  return scope.Close(toV8(settings->getAll()));
}

Handle<Value> SettingsJs::getValue(const Arguments& args) {
  HandleScope scope;

  Settings* settings = &conf();

  QString value = toCpp<QString>(args[0]);
  return scope.Close(toV8(settings->getValue(value)));
}

Handle<Value> SettingsJs::loadJson(const Arguments& args) {
  HandleScope scope;

  Settings* settings = &conf();

  QString url = str(args[0]->ToString());
  settings->loadJson(url);

  return scope.Close(Undefined());
}

Handle<Value> SettingsJs::set(const Arguments& args) {
  HandleScope scope;

  Settings* settings = &conf();

  if (args[0]->IsObject())
  {
    Local<Array> keys = args[0]->ToObject()->GetPropertyNames();
    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(i)->ToString();
      Local<String> v = args[0]->ToObject()->Get(k)->ToString();
      settings->set(str(k), str(v));
    }
  }
  else
  {
    ThrowException(Exception::TypeError(String::New("Expected a dict of settings")));
    return scope.Close(Undefined());
  }

  return scope.Close(Undefined());
}

}
