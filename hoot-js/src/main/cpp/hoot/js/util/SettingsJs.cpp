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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "SettingsJs.h"

// hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/ConfigUtils.h>

#include <hoot/js/JsRegistrar.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/util/StringUtilsJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(SettingsJs)

void SettingsJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();
  Local<Object> settings = Object::New(current);
  exports->Set(context, toV8("Settings"), settings);
  exports->Set(context, toV8("get"),
               FunctionTemplate::New(current, get)->GetFunction(context).ToLocalChecked());
  settings->Set(context, toV8("get"),
                FunctionTemplate::New(current, get)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("set"),
               FunctionTemplate::New(current, set)->GetFunction(context).ToLocalChecked());
  settings->Set(context, toV8("set"),
                FunctionTemplate::New(current, set)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("appendToList"),
               FunctionTemplate::New(current, appendToList)->GetFunction(context).ToLocalChecked());
  settings->Set(context, toV8("appendToList"),
                FunctionTemplate::New(current, appendToList)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("prependToList"),
               FunctionTemplate::New(current, prependToList)->GetFunction(context).ToLocalChecked());
  settings->Set(context, toV8("prependToList"),
                FunctionTemplate::New(current, prependToList)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("removeFromList"),
               FunctionTemplate::New(current, removeFromList)->GetFunction(context).ToLocalChecked());
  settings->Set(context, toV8("removeFromList"),
                FunctionTemplate::New(current, removeFromList)->GetFunction(context).ToLocalChecked());
  exports->Set(context, toV8("replaceInList"),
               FunctionTemplate::New(current, replaceInList)->GetFunction(context).ToLocalChecked());
  settings->Set(context, toV8("replaceInList"),
                FunctionTemplate::New(current, replaceInList)->GetFunction(context).ToLocalChecked());
}

void SettingsJs::get(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  const Settings* settings = &conf();

  QString key = str(args[0]->ToString(context).ToLocalChecked());
  try
  {
    QString value = settings->getString(key);
    args.GetReturnValue().Set(String::NewFromUtf8(current, value.toUtf8().data()).ToLocalChecked());
  }
  catch (const IllegalArgumentException& e)
  {
    LOG_ERROR(e.what());
    args.GetReturnValue().SetUndefined();
  }
}

void SettingsJs::set(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Settings* settings = &conf();

  if (args[0]->IsObject())
  {
    Local<Array> keys =
      args[0]->ToObject(context).ToLocalChecked()->GetPropertyNames(context).ToLocalChecked();
    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked();
      Local<String> v =
        args[0]->ToObject(context)
        .ToLocalChecked()->Get(context, k).ToLocalChecked()->ToString(context).ToLocalChecked();
      LOG_VART(str(k));
      LOG_VART(str(v));
      settings->set(str(k), str(v));
    }
    args.GetReturnValue().SetUndefined();
  }
  else
  {
    args.GetReturnValue().Set(
      current->ThrowException(
        Exception::TypeError(
          String::NewFromUtf8(current, "Expected a dict of settings").ToLocalChecked())));
  }
}

void SettingsJs::appendToList(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Settings* settings = &conf();

  if (args[0]->IsObject())
  {
    Local<Array> keys = args[0]->ToObject(context).ToLocalChecked()->GetPropertyNames(context).ToLocalChecked();
    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked();
      Local<String> v = args[0]->ToObject(context).ToLocalChecked()->Get(context, k).ToLocalChecked()->ToString(context).ToLocalChecked();

      QStringList settingVal = settings->getList(str(k));
      settingVal.append(str(v));
      settings->set(str(k), settingVal);
    }
    args.GetReturnValue().SetUndefined();
  }
  else
  {
    args.GetReturnValue().Set(
      current->ThrowException(
        Exception::TypeError(String::NewFromUtf8(current, "Expected a dict of settings").ToLocalChecked())));
  }
}

void SettingsJs::prependToList(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Settings* settings = &conf();

  if (args[0]->IsObject())
  {
    Local<Array> keys = args[0]->ToObject(context).ToLocalChecked()->GetPropertyNames(context).ToLocalChecked();
    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked();
      Local<String> v = args[0]->ToObject(context).ToLocalChecked()->Get(context, k).ToLocalChecked()->ToString(context).ToLocalChecked();

      QStringList settingVal = settings->getList(str(k));
      settingVal.prepend(str(v));
      settings->set(str(k), settingVal);
    }
    args.GetReturnValue().SetUndefined();
  }
  else
  {
    args.GetReturnValue().Set(
      current->ThrowException(
        Exception::TypeError(String::NewFromUtf8(current, "Expected a dict of settings").ToLocalChecked())));
  }
}

void SettingsJs::removeFromList(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Settings* settings = &conf();

  if (args[0]->IsObject())
  {
    Local<Array> keys = args[0]->ToObject(context).ToLocalChecked()->GetPropertyNames(context).ToLocalChecked();
    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked();
      Local<String> v = args[0]->ToObject(context).ToLocalChecked()->Get(context, k).ToLocalChecked()->ToString(context).ToLocalChecked();

      QStringList settingVal = settings->getList(str(k));
      settingVal.removeAll(str(v));
      settings->set(str(k), settingVal);
    }
    args.GetReturnValue().SetUndefined();
  }
  else
  {
    args.GetReturnValue().Set(
      current->ThrowException(
        Exception::TypeError(String::NewFromUtf8(current, "Expected a dict of settings").ToLocalChecked())));
  }
}

void SettingsJs::replaceInList(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Settings* settings = &conf();

  if (args[0]->IsObject())
  {
    Local<Array> keys = args[0]->ToObject(context).ToLocalChecked()->GetPropertyNames(context).ToLocalChecked();
    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked();
      Local<String> v = args[0]->ToObject(context).ToLocalChecked()->Get(context, k).ToLocalChecked()->ToString(context).ToLocalChecked();

      Settings::replaceListOptionEntryValues(*settings, str(k), str(v).split(";"));
    }
    args.GetReturnValue().SetUndefined();
  }
  else
  {
    args.GetReturnValue().Set(
      current->ThrowException(
        Exception::TypeError(String::NewFromUtf8(current, "Expected a dict of settings").ToLocalChecked())));
  }
}

}
