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
#include "UuidHelperJs.h"

// hoot
#include <hoot/core/util/UuidHelper.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/js/util/DataConvertJs.h>

// QT
#include <QStringList>

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(UuidHelperJs)

UuidHelperJs::UuidHelperJs()
{
}

UuidHelperJs::~UuidHelperJs() {}

void UuidHelperJs::Init(Handle<Object> exports) {
  Handle<Object> helpUuid = Object::New();
  exports->Set(String::NewSymbol("UuidHelper"), helpUuid);
  helpUuid->Set(String::NewSymbol("createUuid"), FunctionTemplate::New(createUuid)->GetFunction());
  helpUuid->Set(String::NewSymbol("createUuid5"), FunctionTemplate::New(createUuid5)->GetFunction());
}


Handle<Value> UuidHelperJs::createUuid(const Arguments& /*args*/) {
  HandleScope scope;

  QString result = UuidHelper::createUuid().toString();

  return scope.Close(toV8(result));
}


Handle<Value> UuidHelperJs::createUuid5(const Arguments& args) {
  HandleScope scope;

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

  return scope.Close(toV8(result));
}



}
