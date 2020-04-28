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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementConverterJs.h"

// hoot
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/js/JsRegistrar.h>
#include <hoot/js/elements/OsmMapJs.h>
#include <hoot/js/elements/ElementJs.h>
#include <hoot/js/io/DataConvertJs.h>
#include <hoot/js/io/StreamUtilsJs.h>
#include <hoot/js/util/StringUtilsJs.h>
#include <hoot/js/util/HootExceptionJs.h> 

using namespace v8;

namespace hoot
{

HOOT_JS_REGISTER(ElementConverterJs)

ElementConverterJs::ElementConverterJs()
{
}

ElementConverterJs::~ElementConverterJs() {}

void ElementConverterJs::Init(Handle<Object> exports)
{
  Isolate* current = exports->GetIsolate();
  HandleScope scope(current);
  Handle<Object> thisObj = Object::New(current);
  exports->Set(String::NewFromUtf8(current, "ElementConverter"), thisObj);
  thisObj->Set(String::NewFromUtf8(current, "calculateLength"),
               FunctionTemplate::New(current, calculateLength)->GetFunction());
}

void ElementConverterJs::calculateLength(const FunctionCallbackInfo<Value>& args)
{
  Isolate* current = args.GetIsolate();
  HandleScope scope(current);
 
  try
  {
    Context::Scope context_scope(current->GetCurrentContext());

    ConstOsmMapPtr m = toCpp<ConstOsmMapPtr>(args[0]);
    ConstElementPtr e = toCpp<ConstElementPtr>(args[1]);

    args.GetReturnValue().Set(toV8(ElementConverter(m).calculateLength(e)));
  }
  catch (const HootException& err)
  {
    LOG_VAR(err.getWhat());
    args.GetReturnValue().Set(current->ThrowException(HootExceptionJs::create(err)));
  }
}

}

