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
 * @copyright Copyright (C) 2015, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "UuidHelperJs.h"

// hoot
#include <hoot/core/util/UuidHelper.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/js/io/DataConvertJs.h>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(UuidHelperJs)

void UuidHelperJs::Init(Local<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Local<Context> context = current->GetCurrentContext();

  Local<Object> helpUuid = Object::New(current);
  exports->Set(context, toV8("UuidHelper"), helpUuid);
  helpUuid->Set(context, toV8("createUuid"),
                FunctionTemplate::New(current, createUuid)->GetFunction(context).ToLocalChecked());
  helpUuid->Set(context, toV8("createUuid5"),
                FunctionTemplate::New(current, createUuid5)->GetFunction(context).ToLocalChecked());
}

void UuidHelperJs::createUuid(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  QString result = UuidHelper::createUuid().toString();

  args.GetReturnValue().Set(toV8(result));
}


void UuidHelperJs::createUuid5(const FunctionCallbackInfo<Value>& args)
{
  HandleScope scope(args.GetIsolate());

  QString result;

  if (args.Length() == 1)
  {
    result = UuidHelper::createUuid5(toCpp<QString>(args[0])).toString();
  }
  else
  {
    result = UuidHelper::createUuid5(toCpp<QString>(args[0]),
                                     QUuid(toCpp<QString>(args[1]))).toString();
  }

  args.GetReturnValue().Set(toV8(result));
}



}
