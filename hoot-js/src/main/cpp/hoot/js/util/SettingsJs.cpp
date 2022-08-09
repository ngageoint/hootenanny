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
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/StringUtils.h>

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
  Maybe<bool> result = exports->Set(context, toV8("Settings"), settings);
  result = exports->Set(context, toV8("clear"),
               FunctionTemplate::New(current, clear)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("clear"),
                FunctionTemplate::New(current, clear)->GetFunction(context).ToLocalChecked());
  result = exports->Set(context, toV8("push"),
               FunctionTemplate::New(current, push)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("push"),
                FunctionTemplate::New(current, push)->GetFunction(context).ToLocalChecked());
  result = exports->Set(context, toV8("pop"),
               FunctionTemplate::New(current, pop)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("pop"),
                FunctionTemplate::New(current, pop)->GetFunction(context).ToLocalChecked());
  result = exports->Set(context, toV8("get"),
               FunctionTemplate::New(current, get)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("get"),
                FunctionTemplate::New(current, get)->GetFunction(context).ToLocalChecked());
  result = exports->Set(context, toV8("set"),
               FunctionTemplate::New(current, set)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("set"),
                FunctionTemplate::New(current, set)->GetFunction(context).ToLocalChecked());
  result = exports->Set(context, toV8("appendToList"),
               FunctionTemplate::New(current, appendToList)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("appendToList"),
                FunctionTemplate::New(current, appendToList)->GetFunction(context).ToLocalChecked());
  result = exports->Set(context, toV8("prependToList"),
               FunctionTemplate::New(current, prependToList)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("prependToList"),
                FunctionTemplate::New(current, prependToList)->GetFunction(context).ToLocalChecked());
  result = exports->Set(context, toV8("removeFromList"),
               FunctionTemplate::New(current, removeFromList)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("removeFromList"),
                FunctionTemplate::New(current, removeFromList)->GetFunction(context).ToLocalChecked());
  result = exports->Set(context, toV8("replaceInList"),
               FunctionTemplate::New(current, replaceInList)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("replaceInList"),
                FunctionTemplate::New(current, replaceInList)->GetFunction(context).ToLocalChecked());
  result = exports->Set(context, toV8("placeAfterInList"),
               FunctionTemplate::New(current, placeAfterInList)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("placeAfterInList"),
                FunctionTemplate::New(current, placeAfterInList)->GetFunction(context).ToLocalChecked());
  result = exports->Set(context, toV8("listContains"),
               FunctionTemplate::New(current, listContains)->GetFunction(context).ToLocalChecked());
  result = settings->Set(context, toV8("listContains"),
                FunctionTemplate::New(current, listContains)->GetFunction(context).ToLocalChecked());
}

void SettingsJs::clear(const FunctionCallbackInfo<Value>& args)
{
  Settings* settings = &conf();
  settings->clear();
  args.GetReturnValue().SetUndefined();
}

void SettingsJs::push(const FunctionCallbackInfo<Value>& args)
{
  Settings* settings = &conf();
  settings->push();
  args.GetReturnValue().SetUndefined();
}

void SettingsJs::pop(const FunctionCallbackInfo<Value>& args)
{
  Settings* settings = &conf();
  settings->pop();
  args.GetReturnValue().SetUndefined();
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
    const QString value = settings->getString(key);
    args.GetReturnValue().Set(String::NewFromUtf8(current, value.toUtf8().data()).ToLocalChecked());
  }
  catch (const IllegalArgumentException& e)
  {
    LOG_ERROR(e.what());
    args.GetReturnValue().SetUndefined();
  }
}

void SettingsJs::listContains(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  const Settings* settings = &conf();

  const QString listOptionKey = str(args[0]->ToString(context).ToLocalChecked());
  const QString strToCheckFor = str(args[1]->ToString(context).ToLocalChecked());

  QStringList listOptionVal = settings->getList(listOptionKey);
  args.GetReturnValue().Set(Boolean::New(current, listOptionVal.contains(strToCheckFor)));
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
    const bool allowDuplicates = toCpp<bool>(args[1]);
    Local<Array> keys =
      args[0]->ToObject(context).ToLocalChecked()->GetPropertyNames(context).ToLocalChecked();
    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked();
      Local<String> v =
        args[0]->ToObject(context)
        .ToLocalChecked()->Get(context, k).ToLocalChecked()->ToString(context).ToLocalChecked();

      QStringList settingVal = settings->getList(str(k));
      if (allowDuplicates || !settingVal.contains(str(v)))
      {
        settingVal.append(str(v));
        settings->set(str(k), settingVal);
      }
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

void SettingsJs::prependToList(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Settings* settings = &conf();

  if (args[0]->IsObject())
  {
    const bool allowDuplicates = toCpp<bool>(args[1]);
    Local<Array> keys =
      args[0]->ToObject(context).ToLocalChecked()->GetPropertyNames(context).ToLocalChecked();
    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked();
      Local<String> v =
        args[0]->ToObject(context)
          .ToLocalChecked()->Get(context, k).ToLocalChecked()->ToString(context).ToLocalChecked();

      QStringList settingVal = settings->getList(str(k));
      if (allowDuplicates || !settingVal.contains(str(v)))
      {
        settingVal.prepend(str(v));
        settings->set(str(k), settingVal);
      }
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

void SettingsJs::placeAfterInList(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Settings* settings = &conf();

  const QString listOptionKey = str(args[0]->ToString(context).ToLocalChecked());
  const QString optionValToReplace = str(args[1]->ToString(context).ToLocalChecked());
  const QString replacementOptionVal = str(args[2]->ToString(context).ToLocalChecked());

  QStringList listOptionVal = settings->getList(listOptionKey);
  if (!StringUtils::insertAfter(listOptionVal, optionValToReplace, replacementOptionVal))
  {
    const QString errorMsg =
      optionValToReplace + " not found in list or " + replacementOptionVal +
      " already exists at specified index for option: " + listOptionKey;
    args.GetReturnValue().Set(
      current->ThrowException(
        Exception::TypeError(
          String::NewFromUtf8(current, errorMsg.toStdString().data()).ToLocalChecked())));
  }
  else
  {
    settings->set(listOptionKey, listOptionVal);
    LOG_VART(settings->getList(listOptionKey));

    args.GetReturnValue().SetUndefined();
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
    Local<Array> keys =
      args[0]->ToObject(context).ToLocalChecked()->GetPropertyNames(context).ToLocalChecked();
    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked();
      Local<String> v =
        args[0]->ToObject(context)
          .ToLocalChecked()->Get(context, k).ToLocalChecked()->ToString(context).ToLocalChecked();

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
        Exception::TypeError(
          String::NewFromUtf8(current, "Expected a dict of settings").ToLocalChecked())));
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
    Local<Array> keys =
      args[0]->ToObject(context).ToLocalChecked()->GetPropertyNames(context).ToLocalChecked();
    for (uint32_t i = 0; i < keys->Length(); i++)
    {
      Local<String> k = keys->Get(context, i).ToLocalChecked()->ToString(context).ToLocalChecked();
      Local<String> v =
        args[0]->ToObject(context)
          .ToLocalChecked()->Get(context, k).ToLocalChecked()->ToString(context).ToLocalChecked();

      Settings::replaceListOptionEntryValues(*settings, str(k), str(v).split(";"));
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

}
